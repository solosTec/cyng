/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/sql.h>
#include <boost/algorithm/string.hpp>

namespace cyng 
{
	namespace sql 
	{
		base::base(meta_table_ptr m, dialect dia, std::stringstream&& os)
			: meta_(m)
			, dialect_(dia)
			, stream_(std::move(os))
		{
			BOOST_ASSERT_MSG(!!meta_, "no meta data");
		}

		bool base::is_valid() const
		{
			return meta_.operator bool();
		}

		std::string base::to_str() const
		{
			return stream_.str();
		}

		std::string base::operator()() const
		{
			return to_str();
		}

		meta_table_ptr base::get_meta() const
		{
			return meta_;
		}

		bool base::do_skip(std::string name) const
		{
			return (SQLITE == dialect_) && boost::algorithm::equals(name, "ROWID");
		}

		bool base::is_tp(column const& col) const
		{
			return !has_feature(dialect_, DATE_TIME_SUPPORT) && (meta_->get_type(col.pos_) == TC_TIME_POINT);
		}

		std::string base::get_placeholder(column const& col) const
		{
			return (is_tp(col))
				? "julianday(?)"
				: "?"
				;
		}

		std::string base::get_full_col_name(column const& col) const
		{
			return (is_tp(col))
				? ("datetime(" + meta_->get_name() + "." + col.name_ + ")")
				: (meta_->get_name() + "." + col.name_)
				;
		}

		command::command(meta_table_ptr m, dialect dia)
			: base(m, dia, std::stringstream())
		{}
		
		void command::clear()
		{
			stream_.str("");
		}
		
		void command::clear(std::string const& str)
		{
			clear();
			stream_
			<< str
			<< ' '
			;
		}
		
		sql_select command::select()
		{
			clear("SELECT");
			return sql_select(meta_, dialect_, std::move(stream_));
		}
		
		sql_create command::create()
		{
			clear("CREATE TABLE");
			return sql_create(meta_, dialect_, std::move(stream_));
		}

		sql_remove command::remove()
		{
			clear("DELETE FROM");
			return sql_remove(meta_, dialect_, std::move(stream_));
		}

		sql_insert command::insert()
		{
			clear("INSERT INTO");
			return sql_insert(meta_, dialect_, std::move(stream_));
		}
		
		sql_update command::update()
		{
			clear("UPDATE");

			stream_
				<< meta_->get_name()
				<< " SET "
				;

			meta_->loop([&](column&& col) {

				if (!col.pk_)
				{
					stream_
						<< col.name_
						<< " = "
						<< get_placeholder(col)
						;
				}
			});

			stream_ << ' ';

			return sql_update(meta_, dialect_, std::move(stream_));
		}

		sql_select::sql_select(meta_table_ptr m, dialect dia, std::stringstream&& os)
			: base(m, dia, std::move(os))
		{}

#if __cplusplus < 201703L 
		sql_select::sql_select(sql_select&& other)
			: base(other.meta_, other.dialect_, std::move(other.stream_))
		{}
#endif
		
		sql_from sql_select::all()
		{
			//
			//	all columns
			//
			bool init_flag = false;
			meta_->loop([this, &init_flag](column&& col) {
				if (!init_flag)
				{
					init_flag = true;
				}
				else
				{
					stream_ << ", ";
				}

				stream_ << get_full_col_name(col);
			});

			stream_ << ' ';
			return sql_from(meta_, dialect_, std::move(stream_));
		}

		sql_from sql_select::count()
		{
			stream_
				<< "COUNT (*) ";
			;
			return sql_from(meta_, dialect_, std::move(stream_));
		}

		sql_from sql_select::count(std::size_t col)
		{
			stream_	
				<< "COUNT ("
				<< meta_->get_name(col - 1)
				<< ") ";
				;
			return sql_from(meta_, dialect_, std::move(stream_));
		}

		sql_from sql_select::sum(std::size_t col)
		{
			stream_
				<< "SUM ("
				<< meta_->get_name(col - 1)
				<< ")";
			;
			return sql_from(meta_, dialect_, std::move(stream_));
		}

		sql_from sql_select::avg(std::size_t col)
		{
			stream_
				<< "AVG ("
				<< meta_->get_name(col - 1)
				<< ")";
			;
			return sql_from(meta_, dialect_, std::move(stream_));
		}

		sql_from sql_select::inner_join_all_on_pk(meta_table_ptr join)
		{
			//
			//	all columns of both tables
			//
			bool init_flag = false;

			//
			//	table (1)
			//
			write_columns(meta_, init_flag);

			//
			//	table (2)
			//
			write_columns(join, init_flag);

			stream_ 
				<< " FROM "
				<< meta_->get_name()
				<< " INNER JOIN "
				<< join->get_name()
				<< " ON "
				;

			//
			//	only one PK is supported
			//
			write_pk(meta_, join);
			return sql_from(meta_, dialect_, std::move(stream_));
		}

		void sql_select::write_columns(meta_table_ptr meta, bool& init_flag)
		{
			meta->loop([&](column&& col) {
				if (!init_flag) {
					init_flag = true;
				}
				else {
					stream_ << ", ";
				}

				stream_ << get_full_col_name(col);
			});
		}

		void sql_select::write_pk(meta_table_ptr meta1, meta_table_ptr meta2)
		{
			BOOST_ASSERT(meta1->has_pk());
			BOOST_ASSERT(meta2->has_pk());
			meta1->loop([&](column&& col) {

				if (col.pk_)
				{
					stream_
						<< meta1->get_name()
						<< col.name_
						;
				}
			});

			stream_ << " = ";

			meta2->loop([&](column&& col) {

				if (col.pk_)
				{
					stream_
						<< meta2->get_name()
						<< col.name_
						;
				}
			});

		}

		sql_from::sql_from(meta_table_ptr m, dialect dia, std::stringstream&& os)
			: base(m, dia, std::move(os))
		{
			stream_ 
				<< "FROM "
				<< m->get_name()
				<< ' '
				;			
		}

#if __cplusplus < 201703L
		sql_from::sql_from(sql_from&& other)
			: base(other.meta_, other.dialect_, std::move(other.stream_))
		{}
#endif
		
		sql_where sql_from::by_key()
		{
			stream_
				<< "WHERE "
				;

			bool init_flag = false;
			meta_->loop([this, &init_flag](column&& col) {

				if (col.pk_)
				{
					if (!init_flag)
					{
						init_flag = true;
					}
					else
					{
						stream_ << " AND ";
					}

					stream_
						<< col.name_
						<< " = ?"
						;
				}
			});

			return sql_where(meta_, dialect_, std::move(stream_));

		}


		sql_order::sql_order(meta_table_ptr m, dialect dia, std::stringstream&& os)
			: base(m, dia, std::move(os))
		{}

#if __cplusplus < 201703L
		sql_order::sql_order(sql_order&& other)
			: base(other.meta_, other.dialect_, std::move(other.stream_))
		{}
#endif
		
		sql_group::sql_group(meta_table_ptr m, dialect dia, std::stringstream&& os)
			: base(m, dia, std::move(os))
		{}

		sql_order sql_group::order_by(std::string const& term)
		{
			stream_
				<< "ORDER BY "
				<< term
				;
			return sql_order(meta_, dialect_, std::move(stream_));
		}

		sql_where::sql_where(meta_table_ptr m, dialect dia, std::stringstream&& os)
			: base(m, dia, std::move(os))
		{}

		sql_order sql_where::order_by(std::string const& term)
		{
			stream_
				<< "ORDER BY "
				<< term
				;
			return sql_order(meta_, dialect_, std::move(stream_));
		}

		sql_create::sql_create(meta_table_ptr m, dialect dia, std::stringstream&& os)
			: base(m, dia, std::move(os))
		{
			if (has_feature(dialect_, IF_NOT_EXISTS))
			{
				stream_ 
				<< "IF NOT EXISTS "
				;
			}
			
			stream_ 
 			<< meta_->get_name()
			<< ' '
			<< '('
			;
			
			//
			//	write column definitions
			//
			write_columns();
			
			//
			//	write primary key constraint
			//
			if (has_pk())
			{
				stream_
				<< ", PRIMARY KEY("
				;
			
				write_pks();
				
				stream_
				<< ")"
				;
			}
			
			//
			//	write (named) unique constraints
			//
// 			const bool has_uc = table_->has_unique_constraints();
// 			if (has_uc)
// 			{
// 				write_unique_constraints(has_pk);
// 			}
			
			stream_ 
			<< ')'
			;
			
		}
		
#if __cplusplus < 201703L
		sql_create::sql_create(sql_create&& other)
			: base(other.meta_, other.dialect_, std::move(other.stream_))
		{}
#endif
		
		
		bool sql_create::has_pk() const
		{
			std::size_t count{ 0u };
			meta_->loop([&](column&& col) {

				if (col.pk_)
				{
					//
					//	with SQLite this prevents creating a column
					//	
					if (!do_skip(col.name_)) {
						++count;
					}
				}
			});

			return count != 0u;
		}

		void sql_create::write_columns()
		{
			bool init_flag = false;
			meta_->loop([this, &init_flag](column&& col){

				if (!do_skip(col.name_)) {
					if (!init_flag)
					{
						init_flag = true;
					}
					else
					{
						stream_ << ", ";
					}

					stream_
						<< col.name_
						<< ' '
						<< get_field_type(dialect_, col.type_, col.width_)
						;
				}
			});
		}
		
		void sql_create::write_pks()
		{
			bool init_flag = false;
			meta_->loop([this, &init_flag](column&& col){
				
				if (col.pk_)
				{
					//
					//	with SQLite this prevents creating a column
					//	
					if (!do_skip(col.name_))
					{
						if (!init_flag)
						{
							init_flag = true;
						}
						else
						{
							stream_ << ", ";
						}

						stream_
							<< col.name_
							;
					}
				}
			});
		}
		
		sql_remove::sql_remove(meta_table_ptr m, dialect dia, std::stringstream&& os)
			: base(m, dia, std::move(os))
		{
			stream_
				<< meta_->get_name()
				<< ' '
				;
		}

		sql_where sql_remove::by_key()
		{
			stream_
				<< "WHERE "
				;

			bool init_flag = false;
			meta_->loop([this, &init_flag](column&& col) {

				if (col.pk_)
				{
					if (!init_flag)
					{
						init_flag = true;
					}
					else
					{
						stream_ << " AND ";
					}

					stream_
						<< col.name_
						<< " = ?"
						;
				}
			});

			return sql_where(meta_, dialect_, std::move(stream_));
		}

		sql_insert::sql_insert(meta_table_ptr m, dialect dia, std::stringstream&& os)
			: base(m, dia, std::move(os))
		{
			stream_ 
			<< m->get_name()
			<< ' '
			<< '('
			;
			
			write_columns();
			
			stream_ 
			<< ") VALUES ("
			;
			
			write_placholders();
			
			stream_ 
			<< ')'
			;
			
		}
		
		void sql_insert::write_columns()
		{
			bool init_flag = false;
			meta_->loop([this, &init_flag](column&& col){
				if (!do_skip(col.name_)) {
					if (!init_flag)
					{
						init_flag = true;
					}
					else
					{
						stream_ << ", ";
					}

					stream_
						<< col.name_
						;
				}
			});
		}
		
		void sql_insert::write_placholders()
		{
			bool init_flag = false;
			meta_->loop([this, &init_flag](column&& col){
				if (!do_skip(col.name_)) {
					if (!init_flag)
					{
						init_flag = true;
					}
					else
					{
						stream_ << ", ";
					}

					stream_ << get_placeholder(col);
				}
			});
		}
		
		sql_update::sql_update(meta_table_ptr m, dialect dia, std::stringstream&& os)
			: base(m, dia, std::move(os))
		{}

		sql_where sql_update::by_key()
		{
			stream_
				<< "WHERE "
				;

			bool init_flag = false;
			meta_->loop([this, &init_flag](column&& col) {

				if (col.pk_)
				{
					if (!init_flag)
					{
						init_flag = true;
					}
					else
					{
						stream_ << " AND ";
					}

					stream_
						<< col.name_
						<< " = ?"
						;
				}
			});

			return sql_where(meta_, dialect_, std::move(stream_));
		}
	}	
}




