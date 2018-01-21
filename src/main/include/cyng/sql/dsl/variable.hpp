/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SQL_DSL_VARIABLE_HPP
#define CYNG_SQL_DSL_VARIABLE_HPP

#include <ostream>
#include <cyng/table/meta_interface.h>
#include <cyng/sql/dialect.h>

namespace cyng 
{
	using namespace table;
	namespace sql
	{
		template < typename T >
		struct variable
		{
			variable(T& v)
			: v_(v)
			{}
			
			void serialize(std::ostream& os, meta_table_ptr, dialect dia) const
			{
				os << *this;
			}
			
			friend std::ostream& operator<<(std::ostream& os, variable const& v)
			{
				os << v.v_;
				return os;
			}
			
			T& v_;
		};
		
		/**
		 * std::string
		 */
		template <>
		struct variable < std::string >
		{
			variable(std::string& v);
			
			void serialize(std::ostream& os, meta_table_ptr, dialect dia) const;

			friend std::ostream& operator<<(std::ostream& os, variable const& c);
			
			std::string& v_;
		};

		/**
		 * boolean
		 */
		template <>
		struct variable < bool >
		{
			variable(bool& b);
			
			void serialize(std::ostream& os, meta_table_ptr, dialect dia) const;
			
			friend std::ostream& operator<<(std::ostream& os, variable const& c);
			
			bool& b_;
		};

		/**
		 * factory
		 */
		template < typename T >
		inline variable< T > make_variable(T& c)
		{
			return variable<T>(c);
		}				
	}	
}

#endif	//	CYNG_SQL_DSL_VARIABLE_HPP



