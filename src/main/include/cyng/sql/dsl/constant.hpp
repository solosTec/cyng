/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SQL_DSL_CONSTANT_HPP
#define CYNG_SQL_DSL_CONSTANT_HPP

#include <ostream>
#include <cyng/store/meta_interface.h>
#include <cyng/sql/dialect.h>

namespace cyng 
{
	using namespace store;
	namespace sql
	{
		template < typename T >
		struct constant 
		{
			constant (T const& c)
			: c_(c)
			{}
			
			void serialize(std::ostream& os, meta_table_ptr, dialect dia) const
			{
				os << *this;
			}
			
			friend std::ostream& operator<<(std::ostream& os, constant const& c)
			{
				os << c.c_;
				return os;
			}
			
			const T c_;
		};
		
		/**
		 * array
		 * const char[6]
		 */
		template < std::size_t N >
		//
		//	This is kinda crazy. Why doesn't gcc convert a C string literal
		//	into a const char array?
		//
		
#if _MSC_VER && (_MSC_VER < 1900)
		struct constant < const char[ N ] >
#else
		struct constant < char[ N ] >
#endif
		{
			static_assert(N > 0, "N to small" );
			constant (const char (&c)[N])
			: c_(c, N - 1)
			{
//				std::cout << typeid(c_).name() << std::endl;
			}
			
			void serialize(std::ostream& os, meta_table_ptr, dialect dia) const
			{
				os << *this;
			}
			
			friend std::ostream& operator<<(std::ostream& os, constant const& c)
			{
				os << '\'' << c.c_ << '\'';
				return os;
			}
			
			const std::string c_;
		};

		/**
		 * std::string
		 */
		template <>
		struct constant < std::string >
		{
			constant(std::string const& c);
			
			void serialize(std::ostream& os, meta_table_ptr, dialect dia) const;

			friend std::ostream& operator<<(std::ostream& os, constant const& c);
			
			const std::string c_;
		};
		
		/**
		 * boolean
		 */
		template <>
		struct constant < bool >
		{
			constant(bool b);
			
			void serialize(std::ostream& os, meta_table_ptr, dialect dia) const;
			
			friend std::ostream& operator<<(std::ostream& os, constant const& c);
			
			const bool b_;
		};
		
		template < typename T >
		inline constant< T > make_constant(T const& c)
		{
			return constant<T>(c);
		}
	}	
}

#endif	//	CYNG_SQL_DSL_CONSTANT_HPP




