/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */

#ifndef CYNG_SQL_DSL_EXPR_OPERATOR_HPP
#define CYNG_SQL_DSL_EXPR_OPERATOR_HPP

#include <ostream>

namespace cyng 
{
	namespace sql
	{
		template < typename LHE, typename RHE >
		//	binary_expression < ops::gt, LHE, RHE >
		auto operator>(LHE lhexp, RHE rhexp) {
			return binary_expression < ops::gt, LHE, RHE >(lhexp, rhexp);
		}

		template < typename LHE, typename RHE >
		//	binary_expression < ops::lt, LHE, RHE >
		auto operator<(LHE lhexp, RHE rhexp) {
			return binary_expression < ops::lt, LHE, RHE >(lhexp, rhexp);
		}

		/**
		 * equal to (==)
		 */
		template < typename LHE, typename RHE > 
		//	binary_expression < ops::eq, LHE, RHE >
		auto operator==(LHE lhexp, RHE rhexp) { 
			return binary_expression < ops::eq, LHE, RHE >(lhexp, rhexp); 
		}
			
		/**
		 * not equal to (!=)
		 */
		template < typename LHE, typename RHE > 
		//	binary_expression < ops::ne, LHE, RHE >
		auto operator!=(LHE lhexp, RHE rhexp) { 
			return binary_expression < ops::ne, LHE, RHE >(lhexp, rhexp); 
		}
			
		/**
		 * Less than or equal to (<=)
		 */
		template < typename LHE, typename RHE > 
		//	binary_expression < ops::le, LHE, RHE >
		auto operator<=(LHE lhexp, RHE rhexp)  { 
			return binary_expression < ops::le, LHE, RHE >(lhexp, rhexp); 
		}
			
		/**
		 * Greater than or equal to (>=)
		 */
		template < typename LHE, typename RHE > 
		//binary_expression < ops::ge, LHE, RHE >
		auto operator>=(LHE lhexp, RHE rhexp) { 
			return binary_expression < ops::ge, LHE, RHE >(lhexp, rhexp); 
		}
			
			
		/**
		 * logical and (&&)
		 */
		template < typename LHE, typename RHE > 
		//	binary_expression < ops::land, LHE, RHE >
		auto operator&&(LHE lhexp, RHE rhexp) { 
			return binary_expression < ops::land, LHE, RHE >(lhexp, rhexp); 
		}
			
		/**
		 * logical or (||)
		 */
		template < typename LHE, typename RHE > 
		//	binary_expression < ops::lor, LHE, RHE >
		auto operator||(LHE lhexp, RHE rhexp) { 
			return binary_expression < ops::lor, LHE, RHE >(lhexp, rhexp); 
		}
	}	
}

#endif	//	CYNG_SQL_DSL_EXPR_OPERATOR_HPP



