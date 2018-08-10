/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/sql/dsl/aggregate.h>
#include <cyng/intrinsics/traits/tag.hpp>

namespace cyng 
{
	namespace sql 
	{
		count::count(std::size_t index)
			: index_(index)
		{}

		void count::serialize(std::ostream& os, meta_table_ptr tbl, dialect dia, bool lhe) const
		{
			if (index_ == 0)
			{
				os << "count(*)";
			}
			else
			{
				os
					<< "count("
					<< tbl->get_name(index_ - 1)
					<< ")"
					;
			}
		}

		sum::sum(std::size_t index)
			: index_(index)
		{}

		void sum::serialize(std::ostream& os, meta_table_ptr tbl, dialect dia, bool lhe) const
		{
			os
				<< "sum("
				<< tbl->get_name(index_ - 1)
				<< ")"
				;		
		}

		average::average(std::size_t index)
			: index_(index)
		{}

		void average::serialize(std::ostream& os, meta_table_ptr tbl, dialect dia, bool lhe) const
		{
			os
				<< "avg("
				<< tbl->get_name(index_ - 1)
				<< ")"
				;
		}


		maximum::maximum(std::size_t index)
			: index_(index)
		{}
		void maximum::serialize(std::ostream& os, meta_table_ptr tbl, dialect dia, bool lhe) const
		{
			const bool b = !has_feature(dia, DATE_TIME_SUPPORT) && (tbl->get_type(index_ - 1) == TC_TIME_POINT);
			if (b && !lhe)
			{
				os << "datetime(";
			}

			os
				<< "max("
				<< tbl->get_name(index_ - 1)
				<< ")"
				;

			if (b && !lhe)
			{
				os << ")";
			}
		}

		minimum::minimum(std::size_t index)
			: index_(index)
		{}
		void minimum::serialize(std::ostream& os, meta_table_ptr tbl, dialect dia, bool lhe) const
		{
			const bool b = !has_feature(dia, DATE_TIME_SUPPORT) && (tbl->get_type(index_ - 1) == TC_TIME_POINT);
			if (b && !lhe)
			{
				os << "datetime(";
			}

			os
				<< "min("
				<< tbl->get_name(index_ - 1)
				<< ")"
				;

			if (b && !lhe)
			{
				os << ")";
			}
		}
	}
}







