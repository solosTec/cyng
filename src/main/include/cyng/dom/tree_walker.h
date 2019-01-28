/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_DOM_TREE_WALKTER_H
#define CYNG_DOM_TREE_WALKTER_H


//#include <cyng/core/object_interface_fwd.h>
#include <cyng/factory/factory.hpp>
#include <cstdint>

namespace cyng 
{
	class tree_walker
	{
	public:
		virtual bool enter_node(std::size_t depth, object const&, std::size_t, std::size_t total, object parent) = 0;
		virtual void leave_node(std::size_t depth, object const&, std::size_t, std::size_t total, object previous) = 0;
	};

	void traverse(object const& node, tree_walker& walker, std::size_t depth = 0, std::size_t idx = 0, std::size_t = 1, object parent = make_object());
	
	/**
	 * crawl over the dom and count all entries 
	 */
	std::size_t dom_counter(object const& node);

}

#endif 	// CYNG_DOM_TREE_WALKTER_H

