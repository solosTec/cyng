/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/sql/dsl/placeholder.h>

namespace cyng 
{
	namespace sql 
	{
		placeholder::placeholder()
		{}
		
		void placeholder::serialize(std::ostream& os, meta_table_ptr tbl, dialect dia, bool is_tp) const
		{
			//	detect special datetime types to use with "julianday()"
			if (is_tp) {
				os
					<< "julianday("
					<< *this
					<< ")"
					;
			}
			else {
				os << *this;
			}
		}

		std::ostream& operator<<(std::ostream& os, placeholder const&)
		{
			//
			//	ToDo: check DATE_TIME_SUPPORT feature
			//	Problem: index not known here
			//
			os << "?";
			return os;
		}

		placeholder make_placeholder()
		{
			return placeholder();
		}

	}	
}







