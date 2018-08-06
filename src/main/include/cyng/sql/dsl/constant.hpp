/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SQL_DSL_CONSTANT_HPP
#define CYNG_SQL_DSL_CONSTANT_HPP

#include <ostream>
#include <cyng/table/meta_interface.h>
#include <cyng/sql/dialect.h>

namespace cyng 
{
	using namespace table;
	namespace sql
	{
		template < typename T >
		struct constant 
		{
			constant (T const& c)
			: c_(c)
			{}
			
			void serialize(std::ostream& os, meta_table_ptr, dialect dia, bool lhe) const
			{
				os << c_;
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
			{}
			
			void serialize(std::ostream& os, meta_table_ptr, dialect dia, bool lhe) const
			{
				os << '\'' << c_ << '\'';
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
			void serialize(std::ostream& os, meta_table_ptr, dialect dia, bool lhe) const;
		
			const std::string c_;
		};
		
		/**
		 * boolean
		 */
		template <>
		struct constant < bool >
		{
			constant(bool b);
			void serialize(std::ostream& os, meta_table_ptr, dialect dia, bool lhe) const;
						
			const bool b_;
		};
		
		/**
		 * timepoint
		 */
		template <>
		struct constant < std::chrono::system_clock::time_point >
		{
			constant(std::chrono::system_clock::time_point b);			
			void serialize(std::ostream& os, meta_table_ptr, dialect dia, bool lhe) const;			
			
			const std::chrono::system_clock::time_point tp_;
		};

		template < typename T >
		inline constant< T > make_constant(T const& c)
		{
			return constant<T>(c);
		}
	}	
}

#endif	//	CYNG_SQL_DSL_CONSTANT_HPP




