/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SQL_DSL_LIST_EXPR_HPP
#define CYNG_SQL_DSL_LIST_EXPR_HPP

#include <string>

namespace cyng 
{
	namespace sql
	{
		/**
		 * Define parameter lists for select statements
		 */
		//template <class... Args>
		//tuple<Args ...> operator , (Args&&... args)
		//{
		//	return std::make_tuple(std::forward<Args>(args)...);
		//}
		
		template< typename... Args >
		struct list_expression
		{
			using param_t = std::tuple<Args ...>;
			const param_t param_;

			list_expression(Args&&... args)
				: param_(std::forward<Args>(args)...)
			{}

			void serialize(std::ostream& os, meta_table_ptr tbl, dialect dia) const
			{
				std::get<0>(param_).serialize(os, tbl, dia);
				os << ", ";
				std::get<1>(param_).serialize(os, tbl, dia);
			}
		};


		template < typename... Args >
		inline list_expression< Args... > make_list(Args&&... args)
		{
			return list_expression< Args... >(std::forward<Args>(args)...);
		}

		/**
		 * list (,)
		 */
		template < typename P1, typename P2 >
		list_expression < P1, P2 >
			operator, (P1&& p1, P2&& p2)
		{ 
			//	P1 is possible a list expression itself:
			//	list_expression<P, Args...>
			//	example:
			//	P2=cyy::sql::column
			//	P1=cyy::sql::list_expression<cyy::sql::column,cyy::sql::column>
			return list_expression<P1, P2>(std::forward<P1>(p1), std::forward<P2>(p2));
			//	reverse order
			//return list_expression<P2, P1>(std::forward<P2>(p2), std::forward<P1>(p1));
		}

		//
		//	Problem: the following function matches for simple values too
		//

		//template < typename P1, typename... Args >
		//list_expression < P1, Args... >
		//	operator, (list_expression<Args...>&& list, P1&& p1)
		//{
		//	//	P1 is possible a list expression itself:
		//	//	list_expression<P, Args...>
		//	//	example:
		//	//	P2=cyy::sql::column
		//	//	P1=cyy::sql::list_expression<cyy::sql::column,cyy::sql::column>


		//	using R = list_expression < P1, Args... >;
		//	using unpack_t = mp_unpack< typename R::param_t >;
		//	//using unpack_t = mp_unpack< std::tuple<P1, Args...> >;

		//	return unpack_t::invoke([p1](Args &&... args)->R {
		//		return R(std::move(p1), std::forward<Args>(args)...);
		//	}, list.param_);

		//}


		/**
		 * list (+)
		 */
		template < typename P1, typename P2 >
		list_expression < P1, P2 >
		operator+ (P1&& p1, P2&& p2)
		{
			return list_expression<P1, P2>(std::forward<P1>(p1), std::forward<P2>(p2));
		}
		
	}	
}

#endif	//	CYNG_SQL_DSL_LIST_EXPR_HPP



