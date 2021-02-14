/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_ALGORITHM_DOM_WALKER_H
#define CYNG_OBJ_ALGORITHM_DOM_WALKER_H

#include <cyng/obj/object.h>
#include <cyng/obj/tag.hpp>

namespace cyng {

	enum class	walker_state {
		FIRST,
		LAST,
		INDETERMINATE
	};

	class dom_walker
	{
	public:
	public:
		virtual ~dom_walker();
		virtual void visit(object const&, type_code, std::size_t depth, walker_state, bool is_vector) = 0;
		virtual void open(type_code, std::size_t depth, std::size_t size) = 0;
		virtual void close(type_code, std::size_t depth, walker_state) = 0;
		virtual void pair(std::size_t, std::size_t depth) = 0;
		virtual void pair(std::string, std::size_t depth) = 0;
	private:

	};

	void traverse(object const&, dom_walker&);

}

#endif 
