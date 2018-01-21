/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include <cyng/sql/dsl/variable.hpp>

namespace cyng 
{
	namespace sql 
	{
		/**
		 * std::string
		 */
		variable < std::string >::variable(std::string& v)
		: v_(v)
		{}
			
		void variable < std::string >::serialize(std::ostream& os, meta_table_ptr, dialect dia) const
		{
			os << *this;
		}
			
		std::ostream& operator<<(std::ostream& os, variable< std::string > const& v)
		{
			os << '\'' << v.v_ << '\'';
			return os;
		}
			
		
		/**
		 * boolean
		 */
		variable < bool >::variable(bool& b)
		: b_(b)
		{}
			
		void variable < bool >::serialize(std::ostream& os, meta_table_ptr, dialect dia) const
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
			
		std::ostream& operator<<(std::ostream& os, variable< bool > const& c)
		{
			//	However, the values TRUE and FALSE are merely aliases for 1 and 0.
			os << (c.b_ ? "TRUE" : "FALSE");
			return os;
		}
	}	
}






