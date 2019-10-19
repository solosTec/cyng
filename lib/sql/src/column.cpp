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
			, name_(std::to_string(index))
			, is_index_(true)
		{
			BOOST_ASSERT_MSG(index != 0, "index out of range");
		}
		
		column::column(std::string name)
			: index_(0u)
			, name_(name)
			, is_index_(false)
		{
			BOOST_ASSERT_MSG(!name_.empty(), "no column name");
		}

		void column::serialize(std::ostream& os, meta_table_ptr tbl, dialect dia, bool lhe) const
		{
			if (is_index_) 
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
			else 
			{
				const bool b = !has_feature(dia, DATE_TIME_SUPPORT) && (tbl->get_type(name_) == TC_TIME_POINT);
				if (b && !lhe)
				{
					os << "datetime(";
				}

				os << name_;

				if (b && !lhe)
				{
					os << ")";
				}
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

		void distinct::serialize(std::ostream& os, meta_table_ptr tbl, dialect dia, bool lhe) const
		{
			const bool b = !has_feature(dia, DATE_TIME_SUPPORT) && (tbl->get_type(index_ - 1) == TC_TIME_POINT);
			if (b && !lhe)
			{
				os << "datetime(";
			}

			os
				<< "DISTINCT "
				<< tbl->get_name(index_ - 1)
				;

			if (b && !lhe)
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







