/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_DOCSCRIPT_LIBRARY_H
#define CYNG_DOCSCRIPT_LIBRARY_H

#include <string>
#include <map>
#include <memory>
#include <initializer_list>

namespace cyng	
{
	namespace docscript
	{
		enum function_type : std::uint16_t {
			NONE_,	//	undefined
			NL_,
			WS_,
			//	environment specific
			ENV_PROCESSED,
			ENV_RAW,
			ENV_DSL,	//	domain specific language
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

			std::string const name_;	//!<	function name
			std::size_t const rvs_;	//!< return values
			function_type const type_;
		};

		using function_map = std::map<std::string, std::shared_ptr<function const>>;

		void insert(function_map&, std::shared_ptr<function const>, std::initializer_list<std::string> names);
		std::shared_ptr<function const> lookup(function_map const&, std::string const& name);

	}
}	//	cyng

#endif	//	CYNG_DOCSCRIPT_LIBRARY_H
