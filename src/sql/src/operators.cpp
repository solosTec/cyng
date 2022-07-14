#include <cyng/sql/dsl/operators.hpp>

namespace cyng 
{
	namespace sql
	{
		std::ostream& operator<<(std::ostream& os, ops::gt)
		{
			os << '>';
			return os;
		}
		std::ostream& operator<<(std::ostream& os, ops::lt)
		{
			os << '<';
			return os;
		}
		std::ostream& operator<<(std::ostream& os, ops::eq)
		{
			os << '=';
			return os;
		}
		
		/**
		 * not equal to (<>)
		 */
		std::ostream& operator<<(std::ostream& os, ops::ne)
		{
			os << '<' << '>';
			return os;
		}
		
		/**
		 * Less than or equal to (<=)
		 */
		std::ostream& operator<<(std::ostream& os, ops::le)
		{
			os << '<' << '=';
			return os;
		}
		
		/**
		 * Greater than or equal to (>=)
		 */
		std::ostream& operator<<(std::ostream& os, ops::ge)
		{
			os << '>' << '=';
			return os;
		}

		/**
		 * Multiplication 
		 */
		std::ostream& operator<<(std::ostream& os, ops::mul)
		{
			os << '*';
			return os;
		}
			
		/**
		 * Division 
		 */
		std::ostream& operator<<(std::ostream& os, ops::div)
		{
			os << '/';
			return os;
		}
			
		/**
		 * Addition  
		 */
		std::ostream& operator<<(std::ostream& os, ops::add)
		{
			os << '+';
			return os;
		}
			
		/**
		 * Subtraction   
		 */
		std::ostream& operator<<(std::ostream& os, ops::sub)
		{
			os << '-';
			return os;
		}
		
		/**
		 * logical and (&& - AND)
		 */
		std::ostream& operator<<(std::ostream& os, ops::land)
		{
			os << "AND";
			return os;
		}

		/**
		 * logical or (|| - OR)
		 */
		std::ostream& operator<<(std::ostream& os, ops::lor)
		{
			os << "OR";
			return os;
		}
	}	
}



