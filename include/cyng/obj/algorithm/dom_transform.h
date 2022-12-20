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

    /**
     * Extract the object with the specified path.
     * The boolean value is true, if the specified path was found.
     */
    std::pair<param_t, bool> extract(param_map_t &pmap, std::vector<std::string> const &path);

    /**
     * Take a parameter map and extract all object values into a vactor.
     * Example:
     * "pin": {
          "1": 46,
          "2": 47,
          "3": 50,
          "4": 53
        } => [46,47,50,53]
     */
    [[nodiscard]] cyng::vector_t extract_vector(cyng::param_map_t &&pmap);

    /**
     * Insert a parameter at the specified path
     */
    bool insert(param_map_t &pmap, std::vector<std::string> const &path, param_t param);
    bool insert(param_map_t &pmap, std::vector<std::string> const &path, object obj);

    /**
     * Move an object to a different path
     */
    bool move(param_map_t &, std::vector<std::string> const &source, std::vector<std::string> const &target);

} // namespace cyng

#endif
