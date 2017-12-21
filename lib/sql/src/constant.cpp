/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/sql/dsl/constant.hpp>

namespace cyng 
{
	namespace sql 
	{
		/**
		 * std::string
		 */
		constant < std::string >::constant (std::string const& c)
		: c_(c)
		{}
			
		void constant < std::string >::serialize(std::ostream& os, meta_table_ptr, dialect dia) const
		{
			os << *this;
		}
			
		std::ostream& operator<<(std::ostream& os, constant< std::string > const& c)
		{
			os << '\'' << c.c_ << '\'';
			return os;
		}
			
		
		/**
		 * boolean
		 */
		constant < bool >::constant (bool b)
		: b_(b)
		{}
			
		void constant < bool >::serialize(std::ostream& os, meta_table_ptr, dialect dia) const
		{
			if (dia == SQLITE)
			{
			os << (b_ ? "1" : "0");
			}
			else 
			{
				os << *this;
			}
		}
			
		std::ostream& operator<<(std::ostream& os, constant< bool > const& c)
		{
			//	However, the values TRUE and FALSE are merely aliases for 1 and 0.
			os << (c.b_ ? "TRUE" : "FALSE");
			return os;
		}
	}	
}






