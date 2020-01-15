/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include <cyng/sql/dsl/variable.hpp>
#include <cyng/io/io_chrono.hpp>

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
			
		void variable < std::string >::serialize(std::ostream& os, meta_table_ptr, dialect dia, bool is_tp) const
		{
			os << '\'' << v_ << '\'';
		}
			
		
		/**
		 * boolean
		 */
		variable < bool >::variable(bool& b)
		: b_(b)
		{}
			
		void variable < bool >::serialize(std::ostream& os, meta_table_ptr, dialect dia, bool lhe) const
		{
			if (dia == SQLITE) 	{
				os << (b_ ? "1" : "0");
			}
			else {
				os << (b_ ? "TRUE" : "FALSE");
			}
		}
			
		/**
		 * timepoint
		 */
		variable < std::chrono::system_clock::time_point >::variable(std::chrono::system_clock::time_point& tp)
		: tp_(tp)
		{}
			
		void variable < std::chrono::system_clock::time_point >::serialize(std::ostream& os, meta_table_ptr, dialect dia, bool lhe) const
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






