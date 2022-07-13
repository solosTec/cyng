/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#include <cyng/scm/win_registry.h>
#include <algorithm>

namespace cyng 
{
	namespace sys 
	{
		/**
		*	@param path full path of registry entry.
		*	@base Handle to a registry base key.
		*	@note that value names are not case sensitive.
		*/
		registry_value::registry_value(std::string const& path, HKEY base)
			: key_(0)
			, base_(base)
		{
			split_path(path);
		}

		/**
		 *	@note that value names are not case sensitive.
		 */
		registry_value::registry_value(std::string const& key_name, std::string const& valueName, HKEY base)
			: key_(0)
			, base_(base)
			, key_name_(key_name)
			, value_name_(valueName)
		{}

		registry_value::registry_value(std::string const& key_name, unsigned idx, HKEY base)
			: key_(0)
			, base_(base)
			, key_name_(key_name)
			, value_name_(std::to_string(idx))
		{}

		/**
		 *	Copy constructor
		 */
		registry_value::registry_value(const registry_value& other)
			: key_(other.key_)
			, base_(other.base_)
			, key_name_(other.key_name_)
			, value_name_(other.value_name_)
		{}

		registry_value::~registry_value()
		{}

		bool registry_value::exists()
		{
			bool returnval(false);

			if (open(false))
			{
				DWORD type, size;
				returnval = query_value(&type, &size, 0);
				close();
			}

			return returnval;
		}

		/**
		*	Removes a named value from the specified registry key.
		*
		*	@note that value names are not case sensitive.
		*/
		void registry_value::remove_value()
		{
			if (open(true))
			{
				::RegDeleteValue(key_, value_name_.c_str());
				close();
			}
		}
		/**
		*	Deletes a subkey and includes all its values.
		*	@note that key names are not case sensitive.
		*	@note On WOW64, 32-bit applications view a registry tree
		*		that is separate from the registry tree
		*		that 64-bit applications view.
		*/
		bool registry_value::remove_key()
		{
			//	ignore errors
			return ERROR_SUCCESS == ::RegDeleteKey(base_, key_name_.c_str());
		}

		bool registry_value::open(bool write_access)
		{
			assert(key_ == 0);

			DWORD disposition(0);
			return (write_access)
				? (ERROR_SUCCESS == ::RegCreateKeyEx(base_
				, key_name_.c_str()
				, 0
				, NULL	//	lpClass can be NULL
				, REG_OPTION_NON_VOLATILE
				, KEY_WRITE
				, 0
				, &key_
				, &disposition))
				: (ERROR_SUCCESS == ::RegOpenKeyEx(base_, key_name_.c_str(), 0, KEY_READ, &key_));
		}

		void registry_value::close(void)
		{
			assert(key_ != 0);
			::RegCloseKey(key_);
			key_ = 0;
		}

		/**
		*	Retrieves the type and data for a specified value name
		*	associated with the open registry key.
		*
		*	@note ::RegGetValue() could be used alternatively.
		*/
		bool registry_value::query_value(DWORD *type, DWORD *size, void* buffer)
		{
			assert(key_ != 0);
			return ::RegQueryValueEx(key_
				, value_name_.c_str()
				, 0	//	reserved
				, type
				, reinterpret_cast<BYTE*>(buffer)
				, size) == ERROR_SUCCESS;
		}

		/**
		*	Sets the data and type of a specified value under the
		*	open registry key.
		*/
		bool registry_value::set_value(DWORD type, DWORD size, const void * buffer)
		{
			assert(key_ != 0);
			return ::RegSetValueEx(key_
				, value_name_.c_str()
				, 0	//	reserved
				, type
				, reinterpret_cast<const BYTE *>(buffer)
				, size) == ERROR_SUCCESS;
		}

		/**
		*	Splits path into key and value name.
		*
		*	@exception{std::exception throws an exception if path doesn not contain
		*		one \\ delimiter at least.}
		*/
		void registry_value::split_path(const std::string & path)
		{
			std::string::size_type pos = path.find_last_of('\\');
			assert(pos != std::string::npos);
			if (pos != std::string::npos)
			{
				key_name_ = path.substr(0, pos);
				value_name_ = path.substr(pos + 1);
			}
			else
			{
				throw std::exception("missing delimiter \\");
			}
		}

		/**
		*	@return key name
		*/
		const std::string registry_value::get_key_name() const
		{
			return key_name_;
		}

		HKEY registry_value::get_base_key() const
		{
			return base_;
		}

		/**
		*	@return size of the longest data component among
		*		the key's values, in bytes.
		*/
		size_t registry_value::query_max_value_length()
		{
			assert(key_ != 0);

			DWORD longestValueDataLength(0L);
			if (ERROR_SUCCESS != ::RegQueryInfoKey(key_
				, NULL
				, NULL
				, (LPDWORD)NULL
				, NULL
				, NULL
				, NULL
				, NULL
				, NULL
				, &longestValueDataLength
				, NULL
				, NULL))
			{
				throw std::exception("RegQueryInfoKey", ::GetLastError());
			}
			return longestValueDataLength;
		}

		//	+-----------------------------------------------------------------+
		//	| registry_string [specialization]
		//	+-----------------------------------------------------------------+
		/**
		*	Constructor
		*/
		registry_string<std::string>::registry_string(const std::string & name, HKEY base)
			: registry_value(name, base)
		{}

		/**
		 *	Constructor
		 */
		registry_string<std::string>::registry_string(std::string const& key_name, std::string const& valueName, HKEY base)
			: registry_value(key_name, valueName, base)
		{}

		registry_string<std::string>::registry_string(std::string const& key_name, unsigned idx, HKEY base)
			: registry_value(key_name, idx, base)
		{}

		registry_string< std::string >::operator std::string()
		{
			std::string returnval;

			if (open(false))
			{
				DWORD type, size;
				if (query_value(&type, &size, 0))
				{
					assert(type == VALUE_TYPE_SZ);
					char * data = new char[size];

					if (query_value(0, &size, data))
					{
						returnval = data;
					}

					delete[] data;
				}

				close();
			}

			return returnval;
		}

		const registry_string< std::string > & registry_string< std::string >::operator=(const std::string & value)
		{
			if (open(true))
			{
				set_value(VALUE_TYPE_SZ, DWORD(value.length() + 1), value.c_str());
				close();
			}

			return *this;
		}

		registry_string< std::string > registry_string< std::string >::operator[](std::string const& valueName)
		{
			return registry_string(get_key_name(), valueName, get_base_key());
		}


		//	+-----------------------------------------------------------------+
		//	| registry_array [definition]
		//	+-----------------------------------------------------------------+
		/**
		*	Constructor
		*/
		registry_array::registry_array(std::string const& path, HKEY base)
			: registry_value(path, base)
		{}

		/**
		*	Constructor
		*/
		registry_array::registry_array(std::string const& key_name, std::string const& value_name, HKEY base)
			: registry_value(key_name, value_name, base)
		{}

		size_t registry_array::read(value_type& value)
		{
			assert(value.empty());

			value.clear();

			if (open(false))
			{
				//	First find out the size of the key
				size_t max_value_length(query_max_value_length());

				//	Allocate some memory to retrieve the data back into
				BYTE* lpBuffer = new BYTE[max_value_length];

				DWORD dataType(VALUE_TYPE_NONE);
				DWORD data_size(static_cast<DWORD>(max_value_length));
				if (query_value(&dataType, &data_size, lpBuffer))
				{
					assert(dataType == VALUE_TYPE_MULTI_SZ);

					char* lpszStrings = (LPTSTR)lpBuffer;
					while (lpszStrings[0] != 0)
					{
						value.push_back(lpszStrings);
						lpszStrings += (::strlen(lpszStrings) + 1);
					}
				}

				delete[] lpBuffer;
				close();
			}

			return value.size();
		}

		void registry_array::write(const value_type& value)
		{
			//	Work out the size of the buffer we will need
			size_t dwSize(0L);
			for (value_const_iterator it = value.begin(); it != value.end(); ++it)
			{
				dwSize += it->length();
				dwSize++;	//	1 extra for each NULL terminator
			}

			//	Need one second NULL for the double NULL at the end
			dwSize++;

			//	Allocate the memory we want
			BYTE* lpBuffer = new BYTE[dwSize];
			::ZeroMemory(lpBuffer, dwSize);

			//	Now copy the strings into the buffer
			int nCurOffset = 0;
			LPTSTR lpszString = (LPTSTR)lpBuffer;

			for (value_const_iterator it = value.begin(); it != value.end(); ++it)
			{
				//		::strcpy( &lpszString[nCurOffset], it->c_str());
				::strcpy_s(&lpszString[nCurOffset], dwSize, it->c_str());
				nCurOffset += it->length();
				nCurOffset++;
			}

			//	Finally write it into the registry
			if (open(true))
			{
				set_value(VALUE_TYPE_MULTI_SZ, dwSize, lpBuffer);
				close();
			}

			//	free up the memory we used
			delete[] lpBuffer;
		}

		bool registry_array::contains(std::string const& comp)
		{
			value_type value;
			read(value);
			return value.end() == std::find(value.begin(), value.end(), comp);
		}

		bool registry_array::operator +=(std::string const& comp)
		{
			value_type value;
			read(value);
			if (value.end() == std::find(value.begin(), value.end(), comp))
			{
				value.push_back(comp);
				write(value);
				return true;
			}

			return false;
		}

		bool registry_array::operator -=(std::string const& comp)
		{
			value_type value;
			read(value);
			if (value.end() != std::find(value.begin(), value.end(), comp))
			{
				value.remove(comp);
				write(value);
				return true;
			}
			return false;
		}

	}	//	sys
}	//	cyng
