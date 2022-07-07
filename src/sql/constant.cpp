#include <cyng/sql/dsl/constant.hpp>
#include <iomanip>

namespace cyng 
{
	namespace sql 
	{
		constant < char const* >::constant(char const* p)
			: value_(p)
		{}

		std::string constant < char const* >::to_string(dialect d, meta_sql const&) const {
			std::stringstream ss;
			ss << '\'' << value_ << '\'';
			return ss.str();
		}

		constant < std::string >::constant (std::string const& s)
		: value_(s)
		{}
			
		std::string constant < std::string >::to_string(dialect d, meta_sql const&) const {
			std::stringstream ss;
			ss << '\'' << value_ << '\'';
			return ss.str();
		}	
		
		/**
		 * boolean
		 */
		constant < bool >::constant (bool b)
		: b_(b)
		{}
			
		std::string constant < bool >::to_string(dialect d, meta_sql const&) const {
			return (d == dialect::SQLITE)
				? (b_ ? "1" : "0")
				: (b_ ? "TRUE" : "FALSE")
				;
		}
			
		/**
		 * timepoint
		 */
		constant < std::chrono::system_clock::time_point >::constant(std::chrono::system_clock::time_point tp)
			: tp_(tp)
		{}
		std::string constant < std::chrono::system_clock::time_point >::to_string(dialect d, meta_sql const&) const {

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






