/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/sql/dsl/column.h>
#include <cyng/intrinsics/traits/tag.hpp>

namespace cyng 
{
	namespace sql 
	{
		column::column (std::size_t index)
		: index_(index)
		{
			BOOST_ASSERT_MSG(index_ != 0, "index out of range");
		}
		
		void column::serialize(std::ostream& os, meta_table_ptr tbl, dialect dia, bool lhe) const
		{
			const bool b = !has_feature(dia, DATE_TIME_SUPPORT) && (tbl->get_type(index_ - 1) == TC_TIME_POINT);
			if (b && !lhe)
			{
				os << "datetime(";
			}

			if (index_ > tbl->size())
			{ 
				os
					<< "index out of range: "
					<< index_
					;
			}
			else
			{
				os
					<< tbl->get_name(index_ - 1)
					;
			}

			if (b && !lhe)
			{
				os << ")";
			}
		}

		column make_column(std::size_t index)
		{
			return column(index);
		}


		distinct::distinct(std::size_t index)
			: index_(index)
		{
			BOOST_ASSERT_MSG(index_ != 0, "index out of range");
		}

		void distinct::serialize(std::ostream& os, meta_table_ptr tbl, dialect dia) const
		{
			const bool b = !has_feature(dia, DATE_TIME_SUPPORT) && (tbl->get_type(index_ - 1) == TC_TIME_POINT);
			if (b)
			{
				os << "datetime(";
			}

			os
				<< "DISTINCT "
				<< tbl->get_name(index_ - 1)
				;

			if (b)
			{
				os << ")";
			}

		}

		distinct make_distinct(std::size_t index)
		{
			return distinct(index);
		}
	}
}







