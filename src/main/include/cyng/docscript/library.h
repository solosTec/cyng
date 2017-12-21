/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_DOCSCRIPT_LIBRARY_H
#define CYNG_DOCSCRIPT_LIBRARY_H

//#include <cyng/docscript/docscript.h>
//#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <initializer_list>

namespace cyng	
{
	namespace docscript
	{
		enum function_type {
			NONE_,	//	undefined
			NL_,
			WS_,
			ENV_,	//	environment specific
		};

		/**
		 *	helper class to provide function meta data
		 */
		struct function
			: std::enable_shared_from_this<function>
		{
			function();
			function(std::string const&, std::size_t, function_type);
			function(function const&);

			std::shared_ptr<function const> get_ptr() const;

			bool is_undefined() const;
			bool is_ws() const;
			bool is_nl() const;

			const std::string name_;	//!<	function name
			const std::size_t rvs_;	//!< return values
			const function_type	type_;
		};

		using function_map = std::map<std::string, std::shared_ptr<function const>>;

		void insert(function_map&, std::shared_ptr<function const>, std::initializer_list<std::string> names);
		std::shared_ptr<function const> lookup(function_map const&, std::string const& name);

	}
}	//	cyng

#endif	//	CYNG_DOCSCRIPT_LIBRARY_H
