/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/obj/algorithm/find.h>
#include <cyng/obj/factory.hpp>
#include <cyng/obj/tag.hpp>
#include <cyng/obj/value_cast.hpp>

#include <boost/algorithm/string.hpp>

namespace cyng {

    namespace {

        /**
         * search by index
         */
        template <typename T> auto search(T const *p, std::size_t idx) -> typename T::const_iterator {
            return std::find_if(std::begin(*p), std::end(*p), [idx](object const &obj) -> bool {
                switch (obj.tag()) {
                case TC_ATTR: return object_cast<attr_t>(obj)->first == idx;
                case TC_UINT32: return *object_cast<std::uint32_t>(obj) == static_cast<std::uint32_t>(idx);
                case TC_UINT64: return *object_cast<std::uint64_t>(obj) == static_cast<std::uint64_t>(idx);
                default: break;
                }
                return false;
            });
        }

        /**
         * search by name
         */
        template <typename T> auto search(T const *p, std::string const &name) -> typename T::const_iterator {
            return std::find_if(std::begin(*p), std::end(*p), [&name](object const &obj) -> bool {
                switch (obj.tag()) {
                case TC_PARAM: return boost::algorithm::equals(object_cast<param_t>(obj)->first, name);
                case TC_STRING: return boost::algorithm::equals(*object_cast<std::string>(obj), name);
                default: break;
                }
                return false;
            });
        }

        /**
         * search by OBIS code
         */
        template <typename T> auto search(T const *p, obis const &code) -> typename T::const_iterator {
            return std::find_if(std::begin(*p), std::end(*p), [&code](object const &obj) -> bool {
                switch (obj.tag()) {
                case TC_PROP: return object_cast<prop_t>(obj)->first == code;
                case TC_OBIS: {
                    auto const ptr = object_cast<obis>(obj);
                    return (ptr != nullptr) ? *object_cast<obis>(obj) == code : false;
                }
                default: break;
                }
                return false;
            });
        }

        template <typename T> object extract(typename T::const_iterator pos) {

            switch ((*pos).tag()) {
            case TC_ATTR: return object_cast<attr_t>(*pos)->second;
            case TC_PARAM: return object_cast<param_t>(*pos)->second;
            case TC_PROP: return object_cast<prop_t>(*pos)->second;
            default: break;
            }
            return *pos; //	return the search index or name
        }

        /**
         * implementation function
         */
        object find(vector_t const *p, std::size_t idx) {
            auto const pos = search(p, idx);
            if (pos != p->end()) {
                return extract<vector_t>(pos);
            } else if (idx < p->size()) {
                return p->at(idx);
            }
            return make_object();
        }

        object find(deque_t const *p, std::size_t idx) {
            auto const pos = search(p, idx);
            if (pos != p->end()) {
                return extract<deque_t>(pos);
            } else if (idx < p->size()) {
                return p->at(idx);
            }
            return make_object();
        }

        object find(tuple_t const *p, std::size_t idx) {
            auto pos = search(p, idx);
            if (pos != p->end()) {
                return extract<tuple_t>(pos);
            } else if (idx < p->size()) {
                pos = p->begin();
                std::advance(pos, idx);
                return *pos; //	clone
            }
            return make_object();
        }

        object find(attr_map_t const *p, std::size_t idx) {
            auto const pos = p->find(idx);
            return (pos != p->end()) ? pos->second : make_object();
        }

        object find(attr_t const *attr, std::size_t idx) { return (attr->first == idx) ? attr->second : make_object(); }

        object find(vector_t const *p, std::string const &name) {
            auto const pos = search(p, name);
            if (pos != p->end()) {
                return extract<vector_t>(pos);
            }
            return make_object();
        }

        object find(deque_t const *p, std::string const &name) {
            auto const pos = search(p, name);
            if (pos != p->end()) {
                return extract<deque_t>(pos);
            }
            return make_object();
        }

        object find(tuple_t const *p, std::string const &name) {
            auto const pos = search(p, name);
            if (pos != p->end()) {
                return extract<tuple_t>(pos);
            }
            return make_object();
        }

        object find(param_map_t const *p, std::string const &name) {
            auto const pos = p->find(name);
            return (pos != p->end()) ? pos->second : make_object();
        }

        object find(param_t const *p, std::string const &name) {
            return (boost::algorithm::equals(p->first, name)) ? p->second : make_object();
        }

        object find(vector_t const *p, obis const &code) {
            auto const pos = search(p, code);
            if (pos != p->end()) {
                return extract<vector_t>(pos);
            }
            return make_object();
        }

        object find(deque_t const *p, obis const &code) {
            auto const pos = search(p, code);
            if (pos != p->end()) {
                return extract<deque_t>(pos);
            }
            return make_object();
        }

        object find(tuple_t const *p, obis const &code) {
            auto const pos = search(p, code);
            if (pos != p->end()) {
                return extract<tuple_t>(pos);
            }
            return make_object();
        }

        object find(prop_map_t const *p, obis const &code) {
            auto const pos = p->find(code);
            return (pos != p->end()) ? pos->second : make_object();
        }

        object find(prop_t const *p, obis const &code) { return (p->first == code) ? p->second : make_object(); }
    } // namespace

    object find(vector_t const &v, std::size_t idx) { return find(&v, idx); }

    object find(deque_t const &v, std::size_t idx) { return find(&v, idx); }

    object find(tuple_t const &v, std::size_t idx) { return find(&v, idx); }

    object find(attr_map_t const &v, std::size_t idx) { return find(&v, idx); }

    object find(attr_t const &attr, std::size_t idx) { return find(&attr, idx); }

    object find(object const &obj, std::size_t idx) {
        switch (obj.tag()) {
        case TC_TUPLE: return find(object_cast<tuple_t>(obj), idx);
        case TC_VECTOR: return find(object_cast<vector_t>(obj), idx);
        case TC_DEQUE: return find(object_cast<deque_t>(obj), idx);
        case TC_ATTR_MAP: return find(object_cast<attr_map_t>(obj), idx);
        case TC_ATTR: return find(object_cast<attr_t>(obj), idx);
        case TC_PARAM_MAP:
        case TC_PARAM:
        default: break;
        }
        return obj;
    }

    object find(vector_t const &v, std::string const &name) { return find(&v, name); }

    object find(deque_t const &v, std::string const &name) { return find(&v, name); }

    object find(tuple_t const &v, std::string const &name) { return find(&v, name); }

    object find(param_map_t const &v, std::string const &name) { return find(&v, name); }

    object find(param_t const &param, std::string const &name) { return find(&param, name); }

    object find(object const &obj, std::string const &name) {
        if (!obj)
            return obj;
        switch (obj.tag()) {
        case TC_TUPLE: return find(object_cast<tuple_t>(obj), name);
        case TC_VECTOR: return find(object_cast<vector_t>(obj), name);
        case TC_DEQUE: return find(object_cast<deque_t>(obj), name);
        case TC_PARAM_MAP: return find(object_cast<param_map_t>(obj), name);
        case TC_PARAM: return find(object_cast<param_t>(obj), name);
        case TC_ATTR_MAP:
        case TC_ATTR:
        default: break;
        }
        return obj;
    }

    std::string find_value(param_map_t c, std::string key, const char *def) {
        return value_cast<std::string>(find(c, key), std::string(def));
    }

    object find(vector_t const &v, obis const &code) { return find(&v, code); }

    object find(deque_t const &v, obis const &code) { return find(&v, code); }

    object find(tuple_t const &v, obis const &code) { return find(&v, code); }

    object find(prop_map_t const &v, obis const &code) { return find(&v, code); }

    object find(prop_t const &param, obis const &code) { return find(&param, code); }

    object find(object const &obj, obis const &code) {
        if (!obj)
            return obj;
        switch (obj.tag()) {
        case TC_TUPLE: return find(object_cast<tuple_t>(obj), code);
        case TC_VECTOR: return find(object_cast<vector_t>(obj), code);
        case TC_DEQUE: return find(object_cast<deque_t>(obj), code);
        case TC_PROP_MAP: return find(object_cast<prop_map_t>(obj), code);
        case TC_PROP: return find(object_cast<prop_t>(obj), code);
        case TC_ATTR_MAP:
        case TC_ATTR:
        case TC_PARAM_MAP:
        case TC_PARAM:
        default: break;
        }
        return obj;
    }

} // namespace cyng
