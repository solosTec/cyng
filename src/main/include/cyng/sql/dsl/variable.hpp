/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SQL_DSL_VARIABLE_HPP
#define CYNG_SQL_DSL_VARIABLE_HPP

#include <ostream>

namespace cyng 
{
	using namespace store;
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
		

		template < typename T >
		inline variable< T > make_variable(T const& c)
		{
			return variable<T>(c);
		}				
	}	
}

#endif	//	CYNG_SQL_DSL_VARIABLE_HPP



