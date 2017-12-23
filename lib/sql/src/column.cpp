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
		
		void column::serialize(std::ostream& os, meta_table_ptr tbl, dialect dia) const
		{
			const bool b = !has_feature(dia, DATE_TIME_SUPPORT) && (tbl->get_type(index_ - 1) == TC_TIME_POINT);
			if (b)
			{
				os << "datetime(";
			}

			os 
			<< tbl->get_name(index_ - 1)
			;

			if (b)
			{
				os << ")";
			}
		}

		std::ostream& operator<<(std::ostream& os, column const& col)
		{
			os << col.index_;
			return os;
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

		std::ostream& operator<<(std::ostream& os, distinct const& col)
		{
			os << col.index_;
			return os;
		}

		distinct make_distinct(std::size_t index)
		{
			return distinct(index);
		}
	}
}






