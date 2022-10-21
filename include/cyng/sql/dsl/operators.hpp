/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
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
		
		/**
		 * greater than (>)
		 */
		std::ostream& operator<<(std::ostream& os, ops::gt);

		/**
		 * less than (<)
		 */
		std::ostream& operator<<(std::ostream& os, ops::lt);

		/**
		 * equal (=)
		 */
		std::ostream& operator<<(std::ostream& os, ops::eq);
		
		/**
		 * not equal to (<>)
		 */
		std::ostream& operator<<(std::ostream& os, ops::ne);
		
		/**
		 * Less than or equal to (<=)
		 */
		std::ostream& operator<<(std::ostream& os, ops::le);
		
		/**
		 * Greater than or equal to (>=)
		 */
		std::ostream& operator<<(std::ostream& os, ops::ge);

		/**
		 * Multiplication 
		 */
		std::ostream& operator<<(std::ostream& os, ops::mul);
			
		/**
		 * Division 
		 */
		std::ostream& operator<<(std::ostream& os, ops::div);
			
		/**
		 * Addition  
		 */
		std::ostream& operator<<(std::ostream& os, ops::add);
			
		/**
		 * Subtraction   
		 */
		std::ostream& operator<<(std::ostream& os, ops::sub);
		
		/**
		 * logical and (&& - AND)
		 */
		std::ostream& operator<<(std::ostream& os, ops::land);

		/**
		 * logical or (|| - OR)
		 */
		std::ostream& operator<<(std::ostream& os, ops::lor);


	}	
}

#endif	//	CYNG_SQL_DSL_OPERATORS_HPP



