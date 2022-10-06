/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/obj/algorithm/dom_transform.h>

#include <cyng/obj/algorithm/add.hpp>
#include <cyng/obj/container_cast.hpp>

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
                default:
                    auto tpl = object_cast<tuple_t>(obj);
                    transform(object_cast<tuple_t>(obj), depth, f);
                    break;
                }
            } break;
            case TC_VECTOR: {
                auto [r, act] = f(obj, depth, tag);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default:
                    auto tpl = object_cast<tuple_t>(obj);
                    transform(object_cast<vector_t>(obj), depth, f);
                    break;
                }
            } break;
            case TC_DEQUE: {
                auto [r, act] = f(obj, depth, tag);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default:
                    auto tpl = object_cast<tuple_t>(obj);
                    transform(object_cast<deque_t>(obj), depth, f);
                    break;
                }
            } break;
            case TC_ATTR_MAP: {
                auto [r, act] = f(obj, depth, tag);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default:
                    auto tpl = object_cast<tuple_t>(obj);
                    transform(object_cast<attr_map_t>(obj), depth, f);
                    break;
                }
            } break;
            case TC_PARAM_MAP: {
                auto [r, act] = f(obj, depth, tag);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default:
                    auto tpl = object_cast<tuple_t>(obj);
                    transform(object_cast<param_map_t>(obj), depth, f);
                    break;
                }
            } break;
            case TC_PROP_MAP: {
                auto [r, act] = f(obj, depth, tag);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default:
                    auto tpl = object_cast<tuple_t>(obj);
                    transform(object_cast<prop_map_t>(obj), depth, f);
                    break;
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
            for (auto pos = vec.begin(); pos != vec.end();) {
                if (transform(*pos, path, f)) {
                    pos = vec.erase(pos);
                } else {
                    ++pos;
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
                //  extend path
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
                default:
                    auto tpl = object_cast<tuple_t>(obj);
                    transform(*tpl, path, f);
                    break;
                }
            } break;
            case TC_VECTOR: {
                auto [r, act] = f(obj, path);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default:
                    auto vec = object_cast<vector_t>(obj);
                    transform(*vec, path, f);
                    break;
                }
            } break;
            case TC_DEQUE: {
                auto [r, act] = f(obj, path);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default:
                    auto deq = object_cast<deque_t>(obj);
                    transform(*deq, path, f);
                    break;
                }
            } break;
            case TC_ATTR_MAP: {
                auto [r, act] = f(obj, path);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default:
                    auto amap = object_cast<attr_map_t>(obj);
                    transform(*amap, path, f);
                    break;
                }
            } break;
            case TC_PARAM_MAP: {
                auto [r, act] = f(obj, path);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default:
                    auto pmap = object_cast<param_map_t>(obj);
                    transform(*pmap, path, f);
                    break;
                }
            } break;
            case TC_PROP_MAP: {
                auto [r, act] = f(obj, path);
                switch (act) {
                case action::REMOVE: return true;
                case action::REPLACE: obj = std::move(r); break;
                case action::NONE:
                default:
                    auto pmap = object_cast<prop_map_t>(obj);
                    transform(*pmap, path, f);
                    break;
                }
            } break;

            case TC_ATTR: {
                auto attr = object_cast<attr_t>(obj);
                transform(*attr, path, f);
            } break;
            case TC_PARAM: {
                auto param = object_cast<param_t>(obj);
                transform(*param, path, f);
            } break;
            case TC_PROP: {
                auto prop = object_cast<prop_t>(obj);
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

} // namespace cyng
