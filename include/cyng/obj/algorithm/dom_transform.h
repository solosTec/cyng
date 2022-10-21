/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_ALGORITHM_DOM_TRANSFORMER_H
#define CYNG_OBJ_ALGORITHM_DOM_TRANSFORMER_H

#include <cyng/obj/object.h>
#include <cyng/obj/tag.hpp>

namespace cyng {

    enum class action {
        NONE,
        REPLACE,
        REMOVE,
    };

    /**
     * Walk down the object tree and apply tranformations.
     * In the callback function the provided object can be used to produce a new object to replace the existing one.
     */
    void transform(object &, std::function<std::pair<object, action>(object const &, std::size_t, type_code)>);

    /**
     * Walk down the parameter map tree and apply tranformations.
     * In the callback function the provided object can be used to produce a new object to replace the existing one.
     */
    void transform(param_map_t &, std::function<std::pair<object, action>(object const &, std::vector<std::string> const &path)>);

    /**
     * Allows to rename path sections
     */
    void rename(param_map_t &, std::vector<std::string> path, std::vector<std::string> const &rep);

} // namespace cyng

#endif
