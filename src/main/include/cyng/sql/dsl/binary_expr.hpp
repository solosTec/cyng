/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SQL_DSL_BINARY_EXPR_HPP
#define CYNG_SQL_DSL_BINARY_EXPR_HPP

#include <string>
#include <ostream>
#include <cyng/sql/dsl/constant.hpp>

namespace cyng 
{
	using namespace table;
	namespace sql
	{
		/**
		 * Map a C++ type to a serializable
		 * DSL type
		 */
		template < typename T >
		struct detect_type
		{
			typedef T type;
		};
		
		template <>
		struct detect_type <int>
		{
			typedef constant<int> type;
		};
		
		template <>
		struct detect_type <bool>
		{
			typedef constant<bool> type;
		};

		template <>
		struct detect_type <char const*>
		{
			typedef constant<std::string> type;
		};

// 		template <>
// 		struct detect_type <std::chrono::system_clock::time_point>
// 		{
// 			typedef time_point type;
// 		};

		/**
		 * @tparam BOP type of binary operator
		 * @tparam LHE left hand side expression
		 * @tparam RHE right hand side expression
		 */
		template< typename BOP, typename LHE, typename RHE >
		struct binary_expression 
		{
			typedef typename detect_type<LHE>::type LHE_type;
			typedef typename detect_type<RHE>::type RHE_type;
			
		public:
			binary_expression (LHE lhexp, RHE rhexp)
			: bop_()
			, lhe_(lhexp)
			, rhe_(rhexp)
			{}
			
			void serialize(std::ostream& os, meta_table_ptr tbl, dialect dia) const
			{
				os 
				<< '('
				;
				
				lhe_.serialize(os, tbl, dia);
				
				os 
				<< ' '
				<< bop_
				<< ' '
				;
				
				rhe_.serialize(os, tbl, dia);
				
				os 
				<< ')'
				;
			}
			
			friend std::ostream& operator<<(std::ostream& os, binary_expression const& expr)
			{ 
				os 
					<< '('
					<< expr.lhe_
					<< ' '
					<< expr.bop_
					<< ' '
					<< expr.rhe_
					<< ')'
					;
				return os;
			} 

		private:
			const BOP	bop_;
			const LHE_type lhe_;
			const RHE_type rhe_;
		};
		
	}	
}

#endif	//	CYNG_SQL_DSL_BINARY_EXPR_HPP



