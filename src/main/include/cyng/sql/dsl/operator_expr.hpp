/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SQL_DSL_OPERATOR_EXPR_HPP
#define CYNG_SQL_DSL_OPERATOR_EXPR_HPP

#include <ostream>

namespace cyng 
{
	namespace sql
	{
		template < typename LHE, typename RHE >
#if defined(__CPP_SUPPORT_N1984)
		auto
#else
		binary_expression < ops::gt, LHE, RHE >
#endif
		operator>(LHE lhexp, RHE rhexp)
		{
			return binary_expression < ops::gt, LHE, RHE >(lhexp, rhexp);
		}

		template < typename LHE, typename RHE >
#if defined(__CPP_SUPPORT_N1984)
		auto
#else
		binary_expression < ops::lt, LHE, RHE >
#endif
		operator<(LHE lhexp, RHE rhexp)
		{
			return binary_expression < ops::lt, LHE, RHE >(lhexp, rhexp);
		}

		/**
		 * equal to (==)
		 */
		template < typename LHE, typename RHE > 
#if defined(__CPP_SUPPORT_N1984)
		auto
#else
		binary_expression < ops::eq, LHE, RHE >
#endif
		operator==(LHE lhexp, RHE rhexp)
		{ 
			return binary_expression < ops::eq, LHE, RHE >(lhexp, rhexp); 
		}
			
		/**
		 * not equal to (!=)
		 */
		template < typename LHE, typename RHE > 
#if defined(__CPP_SUPPORT_N1984)
		auto
#else
		binary_expression < ops::ne, LHE, RHE >
#endif
		operator!=(LHE lhexp, RHE rhexp)
		{ 
			return binary_expression < ops::ne, LHE, RHE >(lhexp, rhexp); 
		}
			
		/**
		 * Less than or equal to (<=)
		 */
		template < typename LHE, typename RHE > 
#if defined(__CPP_SUPPORT_N1984)
		auto
#else
		binary_expression < ops::le, LHE, RHE >
#endif
		operator<=(LHE lhexp, RHE rhexp)  
		{ 
			return binary_expression < ops::le, LHE, RHE >(lhexp, rhexp); 
		}
			
		/**
		 * Greater than or equal to (>=)
		 */
		template < typename LHE, typename RHE > 
#if defined(__CPP_SUPPORT_N1984)
		auto
#else
		binary_expression < ops::ge, LHE, RHE >
#endif
		operator>=(LHE lhexp, RHE rhexp)
		{ 
			return binary_expression < ops::ge, LHE, RHE >(lhexp, rhexp); 
		}
			
			
		/**
		 * logical and (&&)
		 */
		template < typename LHE, typename RHE > 
#if defined(__CPP_SUPPORT_N1984)
		auto
#else
		binary_expression < ops::land, LHE, RHE >
#endif
		operator&&(LHE lhexp, RHE rhexp)
		{ 
			return binary_expression < ops::land, LHE, RHE >(lhexp, rhexp); 
		}
			
		/**
		 * logical or (||)
		 */
		template < typename LHE, typename RHE > 
#if defined(__CPP_SUPPORT_N1984)
		auto
#else
		binary_expression < ops::lor, LHE, RHE >
#endif
		operator||(LHE lhexp, RHE rhexp)
		{ 
			return binary_expression < ops::lor, LHE, RHE >(lhexp, rhexp); 
		}
		
		
	}	
}

#endif	//	CYNG_SQL_DSL_OPERATORS_HPP



