/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_TABLE_META_INTERFACE_H
#define CYNG_TABLE_META_INTERFACE_H

#include <cyng/table/body.hpp>
#include <cyng/intrinsics/sets.h>
#include <string>
#include <memory>
#include <functional>

namespace cyng 
{
	namespace table 
	{
		/**
		 * column description
		 */
		struct column 
		{
			const std::size_t pos_;
			const std::string name_;
			const std::size_t type_;
			const bool pk_;	//!< is primary key
			const std::size_t width_;
			
			column(std::size_t pos, std::string name, std::size_t type, bool pk, std::size_t width)
			: pos_(pos)
			, name_(name)
			, type_(type)
			, pk_(pk)
			, width_(width)
			{}
		};
		
		/**
		 * interface to table meta data.
		 * The interface provides data about all columns (name, data type, width), the primary key(s)
		 * and the table name.
		 * ToDo: support for table index / secondary keys
		 */
		class meta_table_interface
		{
		public:
			/**
			 * @return the table name
			 */
			virtual std::string get_name() const = 0;
			
			/**
			 * @return column count (KEY_SIZE + BODY_SIZE)
			 */
			virtual std::size_t size() const = 0;

			/**
			 * Could throw.
			 * 
			 * @return the name of the specified column
			 */
			virtual std::string get_name(std::size_t) const = 0;
			
			/**
			 * Could throw.
			 * 
			 * @return the data type of the specified column
			 */
			virtual std::size_t get_type(std::size_t) const = 0;

			/**
			 * Could throw.
			 * 
			 * @return the (display) width of the data type
			 */
			virtual std::size_t get_width(std::size_t col) const = 0;
			
			virtual bool check_key(key_type const&) const = 0;
			virtual bool check_record(key_type const&, data_type const&) const = 0;
			
			virtual std::pair<std::size_t, bool> get_body_index(std::size_t idx) const = 0;
			virtual std::pair<std::size_t, bool> get_body_index(std::string const&) const = 0;
			virtual std::pair<std::ptrdiff_t, bool> get_record_index(std::string const& name) const = 0;
			
			virtual bool is_body(std::size_t idx) const = 0;
			virtual bool is_key(std::size_t idx) const = 0;
			
			/**
			 * Convert a key into a parameter map. Usefull for
			 * serialization to JSON, XML, you name it.
			 *
			 * @param idx index in data body
			 * @param obj attribute value
			 */
			virtual param_map_t convert_attr(std::size_t idx, object const& obj) const = 0;
			virtual param_map_t convert_key(key_type const& key) const = 0;
			
			/**
			 * loop over all columns 
			 */
			virtual void loop(std::function<void(column&&)>) const = 0;
			
			/**
			 * @return true if table has a primary key
			 */
			virtual bool has_pk() const = 0;
		};
		
		/**
		 * Define an abstract shared pointer
		 */
		using meta_table_ptr = std::shared_ptr<meta_table_interface>;	
		
	}	//	table	
}

#endif	//	CYNG_TABLE_META_INTERFACE_H




