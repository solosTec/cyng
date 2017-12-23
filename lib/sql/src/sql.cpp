/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/sql.h>

namespace cyng 
{
	namespace sql 
	{
		command::command(meta_table_ptr m, dialect dia)
		: meta_(m)
		, dialect_(dia)
		, stream_()
		{
			
		}
		
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
			return sql_select(meta_, dialect_, stream_);
		}
		
		sql_create command::create()
		{
			clear("CREATE TABLE");
			return sql_create(meta_, dialect_, stream_);
		}

		sql_insert command::insert()
		{
			clear("INSERT INTO");
			return sql_insert(meta_, dialect_, stream_);
		}
		
// 		sql_update command::update()
// 		{
// 			clear("UPDATE");
// 			
// 			stream_
// 			<< meta_->get_name()
// 			<< " SET "
// 			;
// 			
// 			return sql_update(meta_, dialect_, stream_);
// 		}

		std::string command::to_str() const
		{
			return stream_.str();
		}

		
		sql_select::sql_select(meta_table_ptr m, dialect dia, std::ostream& os)
		: meta_(m)
		, dialect_(dia)
		, stream_(os)
		{}

		sql_from::sql_from(meta_table_ptr m, dialect dia, std::ostream& os)
		: meta_(m)
		, dialect_(dia)
		, stream_(os)
		{
			stream_ 
				<< "FROM "
				<< m->get_name()
				<< ' '
				;
			
		}
		
		sql_where::sql_where(meta_table_ptr m, dialect dia, std::ostream& os)
		: meta_(m)
		, dialect_(dia)
		, stream_(os)
		{}

		sql_create::sql_create(meta_table_ptr m, dialect dia, std::ostream& os)
		: meta_(m)
		, dialect_(dia)
		, stream_(os)
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
			if (meta_->has_pk())
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
		
		void sql_create::write_columns()
		{
			bool init_flag = false;
			meta_->loop([this, &init_flag](column&& col){
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
			});
		}
		
		void sql_create::write_pks()
		{
			bool init_flag = false;
			meta_->loop([this, &init_flag](column&& col){
				
				if (col.pk_)
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
			});
		}
		
		sql_insert::sql_insert(meta_table_ptr m, dialect dia, std::ostream& os)
		: meta_(m)
		, dialect_(dia)
		, stream_(os)
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
			});
		}
		
		void sql_insert::write_placholders()
		{
			bool init_flag = false;
			meta_->loop([this, &init_flag](column&& col){
				if (!init_flag)	
				{
					init_flag = true;
				}
				else	
				{
					stream_ << ", ";
				}
				
				if (!has_feature(dialect_, DATE_TIME_SUPPORT) && (meta_->get_type(col.pos_) == TC_TIME_POINT))
				{
					stream_	
					<< "julianday(?)"
					;
				}
				else 
				{
					stream_
					<< '?'
					;
				}
			});
		}
		
		sql_update::sql_update(meta_table_ptr m, dialect dia, std::ostream& os)
		: meta_(m)
		, dialect_(dia)
		, stream_(os)
		{}
	}	
}



