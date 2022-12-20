/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/obj/algorithm/dom_transform.h>

#include <cyng/io/serialize.h>
#include <cyng/obj/algorithm/add.hpp>
#include <cyng/obj/container_cast.hpp>
#include <cyng/obj/container_factory.hpp>

#include <iostream>

namespace cyng {

    namespace {
        using fun_t = std::function<std::pair<object, action>(object &, std::size_t, type_code)>;

        bool transform(object &obj, std::size_t depth, type_code tag, fun_t f);
        void transform(tuple_t *tpl, std::size_t depth, fun_t f);
        void transform(vector_t *vec, std::size_t depth, fun_t f);
        void transform(deque_t *deq, std::size_t depth, fun_t f);
        void transform(attr_map_t *amap, std::size_t depth, fun_t f);
        void transform(param_map_t *pmap, std::size_t depth, fun_t f);
        void transform(prop_map_t *pmap, std::size_t depth, fun_t f);
        void transform(attr_t *attr, std::size_t depth, fun_t f);
        void transform(param_t *param, std::size_t depth, fun_t f);
        void transform(prop_t *prop, std::size_t depth, fun_t f);

        void transform(tuple_t *tpl, std::size_t depth, fun_t f) {
            for (auto pos = tpl->begin(); pos != tpl->end();) {
                auto const tag = pos->tag();
                if (transform(*pos, depth + 1, static_cast<type_code>(tag), f)) {
                    pos = tpl->erase(pos);
                } else {
                    ++pos;
                }
            }
        }

        void transform(vector_t *vec, std::size_t depth, fun_t f) {
            for (auto pos = vec->begin(); pos != vec->end();) {
                auto const tag = pos->tag();
                if (transform(*pos, depth + 1, static_cast<type_code>(tag), f)) {
                    pos = vec->erase(pos);
                } else {
                    ++pos;
                }
            }
        }

        void transform(deque_t *deq, std::size_t depth, fun_t f) {
            for (auto pos = deq->begin(); pos != deq->end();) {
                auto const tag = pos->tag();
                if (transform(*pos, depth + 1, static_cast<type_code>(tag), f)) {
                    pos = deq->erase(pos);
                } else {
                    ++pos;
                }
            }
        }

        void transform(attr_map_t *amap, std::size_t depth, fun_t f) {
            for (auto pos = amap->begin(); pos != amap->end();) {
                auto const tag = pos->second.tag();
                if (transform(pos->second, depth + 1, static_cast<type_code>(tag), f)) {
                    pos = amap->erase(pos);
                } else {
                    ++pos;
                }
            }
        }

        void transform(param_map_t *pmap, std::size_t depth, fun_t f) {
            for (auto pos = pmap->begin(); pos != pmap->end();) {
                auto const tag = pos->second.tag();
                if (transform(pos->second, depth + 1, static_cast<type_code>(tag), f)) {
                    pos = pmap->erase(pos);
                } else {
                    ++pos;
                }
            }
        }

        void transform(prop_map_t *pmap, std::size_t depth, fun_t f) {
            for (auto pos = pmap->begin(); pos != pmap->end();) {
                auto const tag = pos->second.tag();
                if (transform(pos->second, depth + 1, static_cast<type_code>(tag), f)) {
                    pos = pmap->erase(pos);
                } else {
                    ++pos;
                }
            }
        }

        void transform(attr_t *attr, std::size_t depth, fun_t f) {
            auto const tag = attr->second.tag();
            transform(attr->second, depth, static_cast<type_code>(tag), f);
        }

        void transform(param_t *param, std::size_t depth, fun_t f) {
            auto const tag = param->second.tag();
            transform(param->second, depth, static_cast<type_code>(tag), f);
        }

        void transform(prop_t *prop, std::size_t depth, fun_t f) {
            auto const tag = prop->second.tag();
            transform(prop->second, depth, static_cast<type_code>(tag), f);
        }

        bool transform(object &obj, std::size_t depth, type_code tag, fun_t f) {

            switch (tag) {
            case TC_TUPLE: {
                auto [r, act] = f(obj, depth, tag);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default: {
                    auto tpl = object_cast<tuple_t>(obj);
                    transform(object_cast<tuple_t>(obj), depth, f);
                } break;
                }
            } break;
            case TC_VECTOR: {
                auto [r, act] = f(obj, depth, tag);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default: {
                    auto tpl = object_cast<tuple_t>(obj);
                    transform(object_cast<vector_t>(obj), depth, f);
                } break;
                }
            } break;
            case TC_DEQUE: {
                auto [r, act] = f(obj, depth, tag);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default: {
                    auto tpl = object_cast<tuple_t>(obj);
                    transform(object_cast<deque_t>(obj), depth, f);
                } break;
                }
            } break;
            case TC_ATTR_MAP: {
                auto [r, act] = f(obj, depth, tag);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default: {
                    auto tpl = object_cast<tuple_t>(obj);
                    transform(object_cast<attr_map_t>(obj), depth, f);
                } break;
                }
            } break;
            case TC_PARAM_MAP: {
                auto [r, act] = f(obj, depth, tag);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default: {
                    auto tpl = object_cast<tuple_t>(obj);
                    transform(object_cast<param_map_t>(obj), depth, f);
                } break;
                }
            } break;
            case TC_PROP_MAP: {
                auto [r, act] = f(obj, depth, tag);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default: {
                    auto tpl = object_cast<tuple_t>(obj);
                    transform(object_cast<prop_map_t>(obj), depth, f);
                } break;
                }
            } break;
            case TC_ATTR: transform(object_cast<attr_t>(obj), depth, f); break;
            case TC_PARAM: transform(object_cast<param_t>(obj), depth, f); break;
            case TC_PROP: transform(object_cast<prop_t>(obj), depth, f); break;
            default: {
                //  replace/remove operation
                auto [r, act] = f(obj, depth, tag);
                switch (act) {
                case action::NONE: break;
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                default: break;
                }
            } break;
            }
            return false;
        }
    } // namespace

    void transform(object &obj, std::function<std::pair<object, action>(object const &, std::size_t, type_code)> f) {
        auto const tag = obj.tag();
        transform(obj, 0, static_cast<type_code>(tag), f);
    }

    namespace {
        //
        //  implementation
        //
        using fun_pmap_t = std::function<std::pair<object, action>(object const &, std::vector<std::string> path)>;

        bool transform(object &obj, std::vector<std::string> const &path, fun_pmap_t f);
        void transform(tuple_t &tpl, std::vector<std::string> const &path, fun_pmap_t f);
        void transform(vector_t &vec, std::vector<std::string> const &path, fun_pmap_t f);
        void transform(deque_t &dec, std::vector<std::string> const &path, fun_pmap_t f);
        void transform(attr_map_t &amap, std::vector<std::string> const &path, fun_pmap_t f);
        void transform(param_map_t &pmap, std::vector<std::string> const &path, fun_pmap_t f);
        void transform(prop_map_t &pmap, std::vector<std::string> const &path, fun_pmap_t f);
        void transform(attr_t &pmap, std::vector<std::string> const &path, fun_pmap_t f);
        void transform(param_t &pmap, std::vector<std::string> const &path, fun_pmap_t f);
        void transform(prop_t &pmap, std::vector<std::string> const &path, fun_pmap_t f);

        void transform(tuple_t &tpl, std::vector<std::string> const &path, fun_pmap_t f) {
            for (auto pos = tpl.begin(); pos != tpl.end();) {
                if (transform(*pos, path, f)) {
                    pos = tpl.erase(pos);
                } else {
                    ++pos;
                }
            }
        }

        void transform(vector_t &vec, std::vector<std::string> const &path, fun_pmap_t f) {
            std::size_t idx = 0;
            for (auto pos = vec.begin(); pos != vec.end();) {
                //
                // vector enumeration:
                // extend path by index
                //
                auto p = path;
                p.push_back("#" + std::to_string(idx));
                if (transform(*pos, p, f)) {
                    pos = vec.erase(pos);
                } else {
                    ++pos;
                    ++idx;
                }
            }
        }

        void transform(deque_t &deq, std::vector<std::string> const &path, fun_pmap_t f) {
            for (auto pos = deq.begin(); pos != deq.end();) {
                if (transform(*pos, path, f)) {
                    pos = deq.erase(pos);
                } else {
                    ++pos;
                }
            }
        }

        void transform(attr_map_t &amap, std::vector<std::string> const &path, fun_pmap_t f) {
            for (auto pos = amap.begin(); pos != amap.end();) {
                //  extend path by number
                auto p = path;
                p.push_back(std::to_string(pos->first));
                if (transform(pos->second, p, f)) {
                    pos = amap.erase(pos);
                } else {
                    ++pos;
                }
            }
        }

        void transform(param_map_t &pmap, std::vector<std::string> const &path, fun_pmap_t f) {
            for (auto pos = pmap.begin(); pos != pmap.end();) {
                //  extend path by name
                auto p = path;
                p.push_back(pos->first);
                if (transform(pos->second, p, f)) {
                    pos = pmap.erase(pos);
                } else {
                    ++pos;
                }
            }
        }

        void transform(prop_map_t &pmap, std::vector<std::string> const &path, fun_pmap_t f) {
            for (auto pos = pmap.begin(); pos != pmap.end();) {
                //  extend path
                auto p = path;
                p.push_back(to_string(pos->first));
                if (transform(pos->second, p, f)) {
                    pos = pmap.erase(pos);
                } else {
                    ++pos;
                }
            }
        }

        void transform(attr_t &attr, std::vector<std::string> const &path, fun_pmap_t f) {
            auto p = path;
            p.push_back(std::to_string(attr.first));
            transform(attr.second, p, f);
        }

        void transform(param_t &pmap, std::vector<std::string> const &path, fun_pmap_t f) {
            auto p = path;
            p.push_back(pmap.first);
            transform(pmap.second, p, f);
        }

        void transform(prop_t &prop, std::vector<std::string> const &path, fun_pmap_t f) {
            auto p = path;
            p.push_back(to_string(prop.first));
            transform(prop.second, p, f);
        }

        bool transform(object &obj, std::vector<std::string> const &path, fun_pmap_t f) {
            switch (obj.tag()) {
            case TC_TUPLE: {
                auto [r, act] = f(obj, path);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default: {
                    decltype(auto) tpl = object_cast<tuple_t>(obj);
                    transform(*tpl, path, f);
                } break;
                }
            } break;
            case TC_VECTOR: {
                auto [r, act] = f(obj, path);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default: {
                    decltype(auto) vec = object_cast<vector_t>(obj);
                    transform(*vec, path, f);
                } break;
                }
            } break;
            case TC_DEQUE: {
                auto [r, act] = f(obj, path);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default: {
                    decltype(auto) deq = object_cast<deque_t>(obj);
                    transform(*deq, path, f);
                } break;
                }
            } break;
            case TC_ATTR_MAP: {
                auto [r, act] = f(obj, path);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default: {
                    decltype(auto) amap = object_cast<attr_map_t>(obj);
                    transform(*amap, path, f);
                } break;
                }
            } break;
            case TC_PARAM_MAP: {
                auto [r, act] = f(obj, path);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default: {
                    decltype(auto) pmap = object_cast<param_map_t>(obj);
                    transform(*pmap, path, f);
                } break;
                }
            } break;
            case TC_PROP_MAP: {
                auto [r, act] = f(obj, path);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default: {
                    decltype(auto) pmap = object_cast<prop_map_t>(obj);
                    transform(*pmap, path, f);
                } break;
                }
            } break;

            case TC_ATTR: {
                decltype(auto) attr = object_cast<attr_t>(obj);
                transform(*attr, path, f);
            } break;
            case TC_PARAM: {
                decltype(auto) param = object_cast<param_t>(obj);
                transform(*param, path, f);
            } break;
            case TC_PROP: {
                decltype(auto) prop = object_cast<prop_t>(obj);
                transform(*prop, path, f);
            } break;
            default: {
                //  call transformation routine
                auto [r, act] = f(obj, path);
                switch (act) {
                case action::NONE: break;
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                default: break;
                }
            } break;
            }
            return false;
        }

    } // namespace

    void
    transform(param_map_t &pmap, std::function<std::pair<object, action>(object const &, std::vector<std::string> const &path)> f) {
        std::vector<std::string> path;
        transform(pmap, path, f);
    }

    namespace {

        bool insert(
            param_map_t *pmap,
            std::vector<std::string>::const_iterator pos,
            std::vector<std::string>::const_iterator end,
            param_t param) {
            if (pos == end) {
                //
                //  insert
                //
#ifdef _DEBUG
                std::cout << "insert: " << param.first << std::endl;
#endif
                return pmap->insert(param).second;
            }
            auto const name = *pos++;
            auto idx = pmap->find(name);
            if (idx != pmap->end()) {
                //
                //  walk down
                //
                decltype(auto) pm = object_cast<param_map_t>(idx->second);
                if (pm != nullptr) {
                    return insert(pm, pos, end, param);
                } else {
                    // error: name already assigned
                    return false;
                }
            }
            //
            //  create entry
            //
            auto r = pmap->emplace(name, cyng::param_map_factory()());
            return insert(object_cast<param_map_t>(r.first->second), pos, end, param);
        }

        bool insert(
            param_map_t *pmap,
            std::vector<std::string>::const_iterator pos,
            std::vector<std::string>::const_iterator end,
            object obj) {
            if (pos == end) {
                //
                //  not found
                //
#ifdef _DEBUG
                std::cout << "insert: " << obj << std::endl;
#endif
                return false;
            }
            auto const name = *pos++;
            auto idx = pmap->find(name);
            if (idx != pmap->end()) {
                //
                //  walk down
                //
                decltype(auto) pm = object_cast<param_map_t>(idx->second);
                if (pm != nullptr) {
                    return insert(pm, pos, end, obj);
                } else {
                    // error: name already assigned
                    return false;
                }
            }
            //
            //  create entry
            //
            return pmap->emplace(name, obj).second;
        }

        std::pair<param_t, bool>
        extract(param_map_t *pmap, std::vector<std::string>::const_iterator pos, std::vector<std::string>::const_iterator end) {
            if (pos != end) {
#ifdef _DEBUG
                std::cout << "lookup: " << *pos << std::endl;
#endif
                auto idx = pmap->find(*pos);
                if (idx != pmap->end()) {
                    ++pos;
                    if (pos == end) {
#ifdef _DEBUG
                        std::cout << "found: " << cyng::io::to_typed(idx->second) << std::endl;
#endif
                        //  move value
                        auto param = std::move(*idx);
                        //  remove node if found
                        pmap->erase(idx);
                        //  return copy
                        return {param, true};
                    }

                    //
                    //  We're looking only for parameter maps not params
                    //
                    decltype(auto) pm = object_cast<param_map_t>(idx->second);
                    if (pm != nullptr) {
                        return extract(pm, pos, end);
                    }
                } else {
#ifdef _DEBUG
                    std::cout << *pos << " not found" << std::endl;
#endif
                }
            }
            return {param_t{}, false};
        }

    } // namespace

    /**
     * extract the object with the specified path
     */
    std::pair<param_t, bool> extract(param_map_t &pmap, std::vector<std::string> const &path) {
        return extract(&pmap, path.begin(), path.end());
    }

    cyng::vector_t extract_vector(cyng::param_map_t &&pmap) {
        cyng::vector_t vec;
        std::transform(
            pmap.begin(), pmap.end(), std::back_inserter(vec), [](cyng::param_map_t::value_type const &val) { return val.second; });
        return vec;
    }

    /**
     * Insert a parameter at the specified path
     */
    bool insert(param_map_t &pmap, std::vector<std::string> const &path, param_t param) {
        if (!path.empty()) {
            return insert(&pmap, path.begin(), path.end(), param);
        }
        return false;
    }

    bool insert(param_map_t &pmap, std::vector<std::string> const &path, object obj) {
        if (!path.empty()) {
            return insert(&pmap, path.begin(), path.end(), obj);
        }
        return false;
    }

    void rename(param_map_t &pmap, std::vector<std::string> path, std::vector<std::string> const &rep) {
        decltype(auto) r = extract(pmap, path);
        if (r.second) {
            insert(pmap, rep, r.first.second);
#ifdef _DEBUG
            // std::cout << cyng::io::to_plain(cyng::make_object(pmap)) << std::endl;
            //  std::cout << cyng::io::to_typed(cyng::make_object(pmap)) << std::endl;
#endif
        }
    }

    // void insert(param_map_t &, std::vector<std::string> const &path, param_t) {}
    bool move(param_map_t &pmap, std::vector<std::string> const &source, std::vector<std::string> const &target) {
        std::pair<param_t, bool> r = extract(pmap, source);
        if (r.second) {
            return insert(pmap, target, r.first);
        }
        return r.second;
    }

} // namespace cyng
