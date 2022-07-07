/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */

#ifndef CYNG_SQL_DSL_EXPR_BINARY_HPP
#define CYNG_SQL_DSL_EXPR_BINARY_HPP

#include <string>
#include <ostream>
#include <sstream>
#include <cyng/sql/dsl/constant.hpp>
#include <cyng/sql/dsl/operators.hpp>

namespace cyng 
{
	namespace sql
	{
		/**
		 * @tparam BOP type of binary operator
		 * @tparam LHE left hand side expression
		 * @tparam RHE right hand side expression
		 */
		template< typename BOP, typename LHE, typename RHE >
		struct binary_expression 
		{
	
		public:
			binary_expression (LHE lhexp, RHE rhexp)
			: bop_()
			, lhe_(lhexp)
			, rhe_(rhexp)
			{}
			
			std::string to_string(dialect d, meta_sql const& m) const {
				std::stringstream ss;
				ss 
					<< '('
					<< lhe_.to_string(d, m)
					<< ' ' 
					<< bop_ 
					<< ' '
					<< rhe_.to_string(d, m)
					<< ')'
					;
				return ss.str();
			}
			
		private:
			const BOP bop_;
			const LHE lhe_;
			const RHE rhe_;
		};
		
	}	
}

#include <cyng/sql/dsl/expr_operator.hpp>

#endif	//	CYNG_SQL_DSL_EXPR_BINARY_HPP



