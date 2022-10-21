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

    enum class walker_state { FIRST, LAST, INDETERMINATE };

    class dom_walker {
      public:
        virtual ~dom_walker() = default;
        virtual void visit(object const &, type_code, std::size_t depth, walker_state) = 0;
        virtual void open(type_code, std::size_t depth, std::size_t size) = 0;
        virtual void close(type_code, std::size_t depth, walker_state, type_code parent_type) = 0;
        virtual void pair(std::size_t, std::size_t depth) = 0;
        virtual void pair(std::string const &, std::size_t depth) = 0;
        virtual void pair(obis const &, std::size_t depth) = 0;
        virtual void simple(attr_t const &, std::size_t depth, walker_state) = 0;
        virtual void simple(param_t const &, std::size_t depth, walker_state) = 0;
        virtual void simple(prop_t const &, std::size_t depth, walker_state) = 0;
    };

    void traverse(object const &, dom_walker &);
    void traverse(vector_t const &, dom_walker &);
    void traverse(tuple_t const &, dom_walker &);
    void traverse(param_map_t const &, dom_walker &);

} // namespace cyng

#endif
