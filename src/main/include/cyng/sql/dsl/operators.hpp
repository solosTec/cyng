/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SQL_DSL_OPERATORS_HPP
#define CYNG_SQL_DSL_OPERATORS_HPP

#include <ostream>

namespace cyng 
{
	namespace sql
	{
		namespace ops	//	operators
		{	
			//
			//	Relational and equality operators
			//
			
			/**
			 * greater than (>)
			 */
			struct gt
			{};
			
			/**
			 * less than (<)
			 */
			struct  lt
			{};
			
			/**
			 * equal to (==)
			 */
			struct  eq
			{};
			
			/**
			 * not equal to (!=)
			 */
			struct  ne
			{};
			
			/**
			 * Less than or equal to (<=)
			 */
			struct  le
			{};
			
			/**
			 * Greater than or equal to (>=)
			 */
			struct  ge
			{};
			
			//
			//	Multiplicative operators
			//
			
			/**
			 * Multiplication 
			 */
			struct  mul
			{};
			
			/**
			 * Division 
			 */
			struct  div
			{};

			//
			//	Additive operators
			//
			
			/**
			 * Addition  
			 */
			struct  add
			{};
			
			/**
			 * Subtraction   
			 */
			struct  sub
			{};
			
			//
			//	Logical operators
			//
			
			/**
			 * logical and (&&)
			 */
			struct  land
			{};
			
			/**
			 * logical or (||)
			 */
			struct  lor
			{};
			
		}	//	ops
		
		inline std::ostream& operator<<(std::ostream& os, ops::gt)
		{
			os << '>';
			return os;
		}
		inline std::ostream& operator<<(std::ostream& os, ops::lt)
		{
			os << '<';
			return os;
		}
		inline std::ostream& operator<<(std::ostream& os, ops::eq)
		{
			os << '=';
			return os;
		}
		
		/**
		 * not equal to (<>)
		 */
		inline std::ostream& operator<<(std::ostream& os, ops::ne)
		{
			os << '<' << '>';
			return os;
		}
		
		/**
		 * Less than or equal to (<=)
		 */
		inline std::ostream& operator<<(std::ostream& os, ops::le)
		{
			os << '<' << '=';
			return os;
		}
		
		/**
		 * Greater than or equal to (>=)
		 */
		inline std::ostream& operator<<(std::ostream& os, ops::ge)
		{
			os << '>' << '=';
			return os;
		}

		/**
		 * Multiplication 
		 */
		inline std::ostream& operator<<(std::ostream& os, ops::mul)
		{
			os << '*';
			return os;
		}
			
		/**
		 * Division 
		 */
		inline std::ostream& operator<<(std::ostream& os, ops::div)
		{
			os << '/';
			return os;
		}
			
		/**
		 * Addition  
		 */
		inline std::ostream& operator<<(std::ostream& os, ops::add)
		{
			os << '+';
			return os;
		}
			
		/**
		 * Subtraction   
		 */
		inline std::ostream& operator<<(std::ostream& os, ops::sub)
		{
			os << '-';
			return os;
		}
		
		/**
		 * logical and (&& - AND)
		 */
		inline std::ostream& operator<<(std::ostream& os, ops::land)
		{
			os << "AND";
			return os;
		}

		/**
		 * logical or (|| - OR)
		 */
		inline std::ostream& operator<<(std::ostream& os, ops::lor)
		{
			os << "OR";
			return os;
		}

		template < typename LHE, typename RHE > 
		binary_expression < ops::gt, LHE, RHE > 
		operator>(LHE lhexp, RHE rhexp)  
		{ 
			return binary_expression < ops::gt, LHE, RHE >(lhexp, rhexp); 
		}
		
		template < typename LHE, typename RHE > 
		binary_expression < ops::lt, LHE, RHE > 
		operator<(LHE lhexp, RHE rhexp)  
		{ 
			return binary_expression < ops::lt, LHE, RHE >(lhexp, rhexp); 
		}
		
		/**
		 * equal to (==)
		 */
		template < typename LHE, typename RHE > 
		binary_expression < ops::eq, LHE, RHE > 
		operator==(LHE lhexp, RHE rhexp)  
		{ 
			return binary_expression < ops::eq, LHE, RHE >(lhexp, rhexp); 
		}
			
		/**
		 * not equal to (!=)
		 */
		template < typename LHE, typename RHE > 
		binary_expression < ops::ne, LHE, RHE > 
		operator!=(LHE lhexp, RHE rhexp)  
		{ 
			return binary_expression < ops::ne, LHE, RHE >(lhexp, rhexp); 
		}
			
		/**
		 * Less than or equal to (<=)
		 */
		template < typename LHE, typename RHE > 
		binary_expression < ops::le, LHE, RHE > 
		operator<=(LHE lhexp, RHE rhexp)  
		{ 
			return binary_expression < ops::le, LHE, RHE >(lhexp, rhexp); 
		}
			
		/**
		 * Greater than or equal to (>=)
		 */
		template < typename LHE, typename RHE > 
		binary_expression < ops::ge, LHE, RHE > 
		operator>=(LHE lhexp, RHE rhexp)  
		{ 
			return binary_expression < ops::ge, LHE, RHE >(lhexp, rhexp); 
		}
			
		/**
		 * Multiplication 
		 */
		template < typename LHE, typename RHE > 
		binary_expression < ops::mul, LHE, RHE > 
		operator>=(LHE lhexp, RHE rhexp)  
		{ 
			return binary_expression < ops::mul, LHE, RHE >(lhexp, rhexp); 
		}
			
		/**
		 * Division 
		 */
		template < typename LHE, typename RHE > 
		binary_expression < ops::div, LHE, RHE > 
		operator>=(LHE lhexp, RHE rhexp)  
		{ 
			return binary_expression < ops::div, LHE, RHE >(lhexp, rhexp); 
		}


		/**
		 * Addition  
		 */
		template < typename LHE, typename RHE > 
		binary_expression < ops::add, LHE, RHE > 
		operator>=(LHE lhexp, RHE rhexp)  
		{ 
			return binary_expression < ops::add, LHE, RHE >(lhexp, rhexp); 
		}
			
		/**
		 * Subtraction   
		 */
		template < typename LHE, typename RHE > 
		binary_expression < ops::sub, LHE, RHE > 
		operator>=(LHE lhexp, RHE rhexp)  
		{ 
			return binary_expression < ops::sub, LHE, RHE >(lhexp, rhexp); 
		}
			
		/**
		 * logical and (&&)
		 */
		template < typename LHE, typename RHE > 
		binary_expression < ops::land, LHE, RHE > 
		operator&&(LHE lhexp, RHE rhexp)  
		{ 
			return binary_expression < ops::land, LHE, RHE >(lhexp, rhexp); 
		}
			
		/**
		 * logical or (||)
		 */
		template < typename LHE, typename RHE > 
		binary_expression < ops::lor, LHE, RHE > 
		operator||(LHE lhexp, RHE rhexp)  
		{ 
			return binary_expression < ops::lor, LHE, RHE >(lhexp, rhexp); 
		}
		
		
	}	
}

#endif	//	CYNG_SQL_DSL_OPERATORS_HPP



