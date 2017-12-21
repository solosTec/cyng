/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_DOM_TREE_WALKTER_H
#define CYNG_DOM_TREE_WALKTER_H


#include <cyng/core/object_interface_fwd.h>
#include <cstdint>

namespace cyng 
{
	class tree_walker
	{
	public:
		virtual bool current_node(std::size_t depth, object const&) = 0;
	};

	void traverse(object const& node, tree_walker& walker, std::size_t depth = 0);
	
	/**
	 * crawl over the dom and count all entries 
	 */
	std::size_t dom_counter(object const& node);

}

#endif 	// CYNG_DOM_TREE_WALKTER_H

