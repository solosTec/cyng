/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/docscript/library.h>
//#include <iostream>
//#include <string>
//#include <iomanip>
//#include <boost/regex/pending/unicode_iterator.hpp>

namespace cyng	
{
	namespace docscript
	{
		function::function()
			: name_()
			, rvs_(1)
			, type_(NONE_)
		{}

		function::function(std::string const& name, std::size_t rvs, function_type t)
			: name_(name)
			, rvs_(rvs)
			, type_(t)
		{}

		function::function(function const& other)
			: name_(other.name_)
			, rvs_(other.rvs_)
			, type_(other.type_)
		{}

		std::shared_ptr<function const> function::get_ptr() const
		{
			return this->shared_from_this();
		}

		bool function::is_undefined() const
		{
			return type_ == NONE_;
		}
		bool function::is_ws() const
		{
			return type_ == WS_;
		}
		bool function::is_nl() const
		{
			return type_ == NL_;
		}


		void insert(function_map& map, std::shared_ptr<function const> ptr, std::initializer_list<std::string> names)
		{
			for (auto name : names)
			{ 
				map.emplace(name, ptr);
			}
			map.emplace(ptr->name_, ptr);
		}

		std::shared_ptr<function const> lookup(function_map const& map, std::string const& name)
		{
			auto pos = map.find(name);
			return (pos == map.end())
				? std::make_shared<function>(name, 1, NONE_)
				: pos->second->get_ptr()
				;
		}

	}
}	//	cyng


