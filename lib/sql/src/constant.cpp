/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/sql/dsl/constant.hpp>
#include <cyng/io/io_chrono.hpp>

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
			
		void constant < std::string >::serialize(std::ostream& os, meta_table_ptr, dialect dia, bool lhe) const
		{
			os << '\'' << c_ << '\'';
		}	
		
		/**
		 * boolean
		 */
		constant < bool >::constant (bool b)
		: b_(b)
		{}
			
		void constant < bool >::serialize(std::ostream& os, meta_table_ptr, dialect dia, bool lhe) const
		{
			if (dia == SQLITE) 	{
				os << (b_ ? "1" : "0");
			}
			else {
				//	However, the values TRUE and FALSE are merely aliases for 1 and 0.
				os << (b_ ? "TRUE" : "FALSE");
			}
		}
			
		/**
		 * timepoint
		 */
		constant < std::chrono::system_clock::time_point >::constant (std::chrono::system_clock::time_point tp)
		: tp_(tp)
		{}
			
		void constant < std::chrono::system_clock::time_point >::serialize(std::ostream& os, meta_table_ptr, dialect dia, bool lhe) const
		{
			if (!has_feature(dia, DATE_TIME_SUPPORT)) {
				os
					<< "julianday('"
					<< to_str(tp_)
					<< "')"
					;
			}
			else {
				os << to_str(tp_);
			}
		}

	}	
}






