/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */

#ifndef CYNG_SQL_DSL_VARIABLE_HPP
#define CYNG_SQL_DSL_VARIABLE_HPP

#include <cyng/store/meta.h>
#include <cyng/sql/dialect.h>

#include <ostream>
#include <chrono>

namespace cyng 
{
	namespace sql
	{
		template < typename T >
		struct variable
		{
			variable(T& v)
			: value_(v)
			{}
			
			std::string to_string(dialect d, meta_sql const&) const {
				std::stringstream ss;
				ss << value_;
				return ss.str();
			}
			
			T& value_;
		};
		
		/**
		 * std::string
		 */
		template <>
		struct variable < std::string >
		{
			variable(std::string& v);	
			std::string to_string(dialect d, meta_sql const&) const;
		
			std::string& value_;
		};

		/**
		 * boolean
		 */
		template <>
		struct variable < bool >
		{
			variable(bool& b);	
			std::string to_string(dialect d, meta_sql const&) const;
						
			bool& b_;
		};

		/**
		 * timepoint
		 */
		template <>
		struct variable < std::chrono::system_clock::time_point >
		{
			variable(std::chrono::system_clock::time_point& tp);
			std::string to_string(dialect d, meta_sql const&) const;
			
			std::chrono::system_clock::time_point& tp_;
		};


		/**
		 * factory - only references allowed
		 */
		template < typename T >
		inline variable< T > make_variable(T& ref) {
			return variable<T>(ref);
		}
	}	
}

#endif	//	CYNG_SQL_DSL_VARIABLE_HPP



