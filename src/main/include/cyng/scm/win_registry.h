/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_SCM_WIN_REGISTRY_H
#define CYNG_SCM_WIN_REGISTRY_H

/**	@file win_registry.h
*
*	Sample:
*
*	@code
*	typedef registry_string<double> regdouble;
*	typedef registry_binary<RECT> regrect;
*	typedef registry_u32<bool> regbool;
*	typedef registry_binary<POINT> regpoint;
*	typedef registry_string<std::string> regstring;
*
*	int main()
*	{
*		regdouble d("Software\\RegValueTest\\double", HKEY_CURRENT_USER);
*		regrect r("Software\\RegValueTest\\rect", HKEY_CURRENT_USER);
*		regbool b("Software\\RegValueTest\\bool", HKEY_CURRENT_USER);
*		regpoint p("Software\\RegValueTest\\point", HKEY_CURRENT_USER);
*		regstring s("Software\\RegValueTest\\string", HKEY_CURRENT_USER);
*
*		RECT testrect = {2,3,4,5};
*		POINT testpoint = {99,77};
*
*		r = testrect;
*		d = 98.888;
*		b = true;
*		p = testpoint;
*		s = "hejsan hoppsan";
*
*		bool bb = b;
*		double dd = d;
*		RECT rr = r;
*		POINT pp = p;
*		std::string ss = s;
*
*		std::cout << bb << std::endl;
*		std::cout << dd << std::endl;
*		std::cout << rr.left << " " << rr.top << " "<< rr.right << " "<< rr.bottom << std::endl;
*		std::cout << pp.x << " " << pp.y << std::endl;
*		std::cout << ss << std::endl;
*
*		r.remove_value();
*		d.remove_value();
*		b.remove_value();
*		p.remove_value();
*		s.remove_value();
*
*		s.remove_key();
*
*		return 0;
*	}
*	@endcode
*/
#pragma once
#include <boost/asio.hpp>

#include <sstream>
#include <iomanip>
#include <assert.h>
#include <list>

namespace cyng 
{
	namespace sys 
	{

		//	+-----------------------------------------------------------------+
		//	| registry_value [declaration]
		//	+-----------------------------------------------------------------+
		/**
		*	Encapsulating the windows registry.
		*
		*	The following base keys are supported:
		*	@li HKEY_CLASSES_ROOT
		*	@li HKEY_CURRENT_CONFIG
		*	@li HKEY_CURRENT_USER
		*	@li HKEY_LOCAL_MACHINE
		*	@li HKEY_USERS
		*/
		class registry_value
		{
		public:
			enum ValueType
			{
				VALUE_TYPE_NONE = REG_NONE,				//!< No value type
				VALUE_TYPE_SZ = REG_SZ,				//!< null terminated string
				VALUE_TYPE_EXPAND_SZ = REG_EXPAND_SZ,		//!< null terminated string (with environment variable references)
				VALUE_TYPE_BINARY = REG_BINARY,			//!< Free form binary
				VALUE_TYPE_U32 = REG_DWORD,			//!< 32-bit number
				VALUE_TYPE_U32BE = REG_DWORD_BIG_ENDIAN,	//!< 32-bit number
				VALUE_TYPE_LINK = REG_LINK,				//!< Symbolic Link (unicode)
				VALUE_TYPE_MULTI_SZ = REG_MULTI_SZ,			//!< Array of null-terminated strings, terminated by two null characters.
				VALUE_TYPE_U64 = REG_QWORD,			//!< 64-bit number
				//VALUE_TYPE_RESOURCE_LIST= REG_RESOURCE_LIST,	// Resource list in the resource map
				//VALUE_TYPE_FULL_RESOURCE_DESCRIPTOR	= REG_FULL_RESOURCE_DESCRIPTOR,	// Resource list in the hardware description
				//VALUE_TYPE_RESOURCE_REQUIREMENTS_LIST	= REG_RESOURCE_REQUIREMENTS_LIST,
			};

		public:
			registry_value(const std::string&, HKEY);
			registry_value(const std::string&, const std::string&, HKEY);
			registry_value(const registry_value&);
			virtual ~registry_value();

			bool exists();
			void remove_value();
			bool remove_key(void);

			const std::string get_key_name() const;
			HKEY get_base_key() const;

		protected:
			bool open(bool);
			void close();
			bool query_value(DWORD*, DWORD*, void*);
			bool set_value(DWORD, DWORD, const void*);
			size_t query_max_value_length();

		private:
			void split_path(const std::string&);

			HKEY key_;
			HKEY base_;

			std::string value_name_;
			std::string key_name_;
		};


		//	+-----------------------------------------------------------------+
		//	| registry_string [declaration]
		//	+-----------------------------------------------------------------+
		/**
		*	Stores all data types <T> as string.
		*/
		template< class T >
		class registry_string : public registry_value
		{
		public:
			registry_string(const std::string& path, HKEY base)
				: registry_value(path, base)
			{}
			registry_string(const std::string& key_name, const std::string& value_name, HKEY base)
				: registry_value(key_name, value_name, base)
			{}

			operator T()
			{
				TY_STR_STREAM ss;
				T returnval = T();

				if (open(false) == true)
				{
					TY_MODE type(VALUE_TYPE_NONE), size(0);
					if (query_value(&type, &size, 0))
					{
						assert(type == VALUE_TYPE_SZ);
						char * data = new char[size];

						if (query_value(0, &size, data))
						{
							ss.str(data);
							ss >> returnval;
						}

						delete[] data;
					}

					close();
				}

				return returnval;
			}

			const registry_string & operator=(const T & value)
			{
				if (open(true))
				{
					std::stringstream ss;
					ss
						<< std::setprecision(20)
						<< value;
					std::string data = ss.str();

					set_value(VALUE_TYPE_SZ, TY_SIZE(data.length() + 1), data.c_str());
					close();
				}

				return *this;
			}

			registry_string operator[](const std::string& valueName)
			{
				return registry_string(get_key_name(), valueName, get_base_key());
			}
		};


		//	+-----------------------------------------------------------------+
		//	| registry_string [specialization]
		//	+-----------------------------------------------------------------+
		/**
		*	Stores data type <std::string> as string
		*/
		template < >
		class registry_string< std::string > : public registry_value
		{
		public:
			registry_string(const std::string&, HKEY);
			registry_string(const std::string&, const std::string&, HKEY);

			operator std::string();
			const registry_string & operator=(const std::string&);
			registry_string operator[](const std::string&);
		};

		//	+-----------------------------------------------------------------+
		//	| registry_u32 [declaration]
		//	+-----------------------------------------------------------------+
		/**
		*	Stores all data types as 32 bit integer
		*/
		template< class T >
		class registry_u32 : public registry_value
		{
		public:
			registry_u32(const std::string & name, HKEY base)
				: registry_value(name, base)
			{}
			registry_u32(const std::string& keyName, const std::string& valueName, HKEY base)
				: registry_value(keyName, valueName, base)
			{}

			operator T()
			{
				DWORD returnval = DWORD();

				if (open(false))
				{
					DWORD type, size(sizeof(DWORD));
					if (query_value(&type, &size, &returnval))
					{
						assert(type == VALUE_TYPE_U32);
					}

					close();
				}

				return T(returnval);
			}

			const registry_u32 & operator=(const T & value)
			{
				if (open(true))
				{
					DWORD data(value);
					set_value(VALUE_TYPE_U32, sizeof(DWORD), &data);
					close();
				}

				return *this;
			}

			registry_u32 operator[](const std::string& valueName)
			{
				return registry_u32(get_key_name(), valueName, get_base_key());
			}
		};


		//	+-----------------------------------------------------------------+
		//	| registry_binary [declaration]
		//	+-----------------------------------------------------------------+
		/**
		*	Stores all data types as binary.
		*
		*	@note works for PODs types.
		*/
		template < class T >
		class registry_binary : public registry_value
		{
		public:
			registry_binary(const std::string & name, HKEY base)
				: registry_value(name, base)
			{
				;
			}

			operator T()
			{
				T returnval = T();

				if (open(false))
				{
					DWORD type, size = sizeof(T);
					if (query_value(&type, &size, &returnval) == true)
					{
						assert(type == VALUE_TYPE_BINARY);
					}

					close();
				}

				return returnval;
			}

			const registry_binary & operator=(const T & value)
			{
				if (open(true))
				{
					set_value(VALUE_TYPE_BINARY, sizeof(T), &value);
					close();
				}
				return *this;
			}
		};

		//	+-----------------------------------------------------------------+
		//	| registry_array [specialization]
		//	+-----------------------------------------------------------------+
		/**
		*	Stores and read lists of strings
		*/
		class registry_array : public registry_value
		{
		public:
			typedef std::list< std::string >	value_type;
			typedef value_type::iterator		value_iterator;
			typedef value_type::const_iterator	value_const_iterator;

		public:
			registry_array(const std::string&, HKEY);
			registry_array(const std::string&, const std::string&, HKEY);
			size_t read(value_type&);
			void write(const value_type&);
			bool contains(const std::string&);
			bool operator +=(const std::string&);
			bool operator -=(const std::string&);
		};

	}	//	sys
}	//	cyng

#endif
