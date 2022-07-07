#include <cyng/sql/dsl/variable.hpp>
#include <iomanip>

namespace cyng 
{
	namespace sql 
	{
		/**
		 * std::string
		 */
		variable < std::string >::variable(std::string& v)
		: value_(v)
		{}
			
		std::string variable < std::string >::to_string(dialect d, meta_sql const&) const {
			std::stringstream ss;
			ss << '\'' << value_ << '\'';
			return ss.str();
		}
			
		
		/**
		 * boolean
		 */
		variable < bool >::variable(bool& b)
		: b_(b)
		{}
			
		std::string variable < bool >::to_string(dialect d, meta_sql const&) const {
			return (d == dialect::SQLITE)
				? (b_ ? "1" : "0")
				: (b_ ? "TRUE" : "FALSE")
				;
		}
			
		/**
		 * timepoint
		 */
		variable < std::chrono::system_clock::time_point >::variable(std::chrono::system_clock::time_point& tp)
		: tp_(tp)
		{}
			
		std::string variable < std::chrono::system_clock::time_point >::to_string(dialect d, meta_sql const&) const {
			std::time_t const tt = std::chrono::system_clock::to_time_t(tp_);
			auto tm = *std::localtime(&tt);

			std::stringstream ss;
			if (dialect::SQLITE == d) {
				ss
					<< "julianday('"
					<< std::put_time(&tm, "%F %T")
					<< "')"
					;
			}
			else {
				ss << std::put_time(&tm, "%F %T");
			}
			return ss.str();
		}

	}	
}






