/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/obj/algorithm/dom_walker.h>
#include <cyng/obj/factory.hpp>
#include <cyng/obj/container_cast.hpp>

namespace cyng {

	dom_walker::~dom_walker()
	{}

	namespace {
		walker_state get_state(std::size_t index, std::size_t size) {
			if (index == size)	return walker_state::LAST; 
			else if (index == 1)	return walker_state::FIRST;
			return walker_state::INDETERMINATE;
		}
		void traverse(dom_walker& walker, object const& obj, std::size_t depth, walker_state state, bool is_vector);
		void traverse(dom_walker& walker, tuple_t tpl, std::size_t depth, walker_state state, bool is_vector);
		void traverse(dom_walker& walker, vector_t vec, std::size_t depth, walker_state state, bool is_vector);
		void traverse(dom_walker& walker, deque_t deq, std::size_t depth, walker_state state, bool is_vector);
		void traverse(dom_walker& walker, attr_map_t amap, std::size_t depth, walker_state state, bool is_vector);
		void traverse(dom_walker& walker, param_map_t pmap, std::size_t depth, walker_state state, bool is_vector);
		void traverse(dom_walker& walker, attr_t attr, std::size_t depth, walker_state state, bool is_vector);
		void traverse(dom_walker& walker, param_t param, std::size_t depth, walker_state state, bool is_vector);

		void traverse(dom_walker& walker, tuple_t tpl, std::size_t depth, walker_state state, bool is_vector) {
			walker.open(TC_TUPLE, depth, tpl.size());
			std::size_t index{ 0 };
			for (auto const& obj : tpl) {
				index++;
				traverse(walker, obj, depth + 1, get_state(index, tpl.size()), false);
			}
			walker.close(TC_TUPLE, depth, state);
		}

		void traverse(dom_walker& walker, vector_t vec, std::size_t depth, walker_state state, bool is_vector) {
			walker.open(TC_VECTOR, depth, vec.size());
			std::size_t index{ 0 };
			for (auto const& obj : vec) {
				index++;
				traverse(walker, obj, depth + 1, get_state(index, vec.size()), true);
			}
			walker.close(TC_VECTOR, depth, state);
		}

		void traverse(dom_walker& walker, deque_t deq, std::size_t depth, walker_state state, bool is_vector) {
			walker.open(TC_DEQUE, depth, deq.size());
			std::size_t index{ 0 };
			for (auto const& obj : deq) {
				index++;
				traverse(walker, obj, depth + 1, get_state(index, deq.size()), false);
			}
			walker.close(TC_DEQUE, depth, state);
		}

		void traverse(dom_walker& walker, attr_map_t amap, std::size_t depth, walker_state state, bool is_vector) {
			walker.open(TC_ATTR_MAP, depth, amap.size());
			std::size_t index{ 0 };
			for (auto const& attr : amap) {
				index++;
				traverse(walker, attr, depth + 1, get_state(index, amap.size()), false);
			}
			walker.close(TC_ATTR_MAP, depth, state);
		}

		void traverse(dom_walker& walker, param_map_t pmap, std::size_t depth, walker_state state, bool is_vector) {
			walker.open(TC_PARAM_MAP, depth, pmap.size());
			std::size_t index{ 0 };
			for (auto const& param : pmap) {
				index++;
				traverse(walker, param, depth + 1, get_state(index, pmap.size()), false);
			}
			walker.close(TC_PARAM_MAP, depth, state);
		}

		void traverse(dom_walker& walker, attr_t attr, std::size_t depth, walker_state state, bool is_vector) {
			walker.pair(attr.first, depth);
			traverse(walker, attr.second, depth, state, false);
		}

		void traverse(dom_walker& walker, param_t param, std::size_t depth, walker_state state, bool is_vector) {
			walker.pair(param.first, depth);
			traverse(walker, param.second, depth, state, false);
		}

		void traverse(dom_walker& walker, object const& obj, std::size_t depth, walker_state state, bool is_vector) {

			auto const tag = obj.rtti().tag();
			switch (tag) {
			case TC_TUPLE:
				traverse(walker, container_cast<tuple_t>(obj), depth, state, is_vector);
				break;
			case TC_VECTOR:
				traverse(walker, container_cast<vector_t>(obj), depth, state, is_vector);
				break;
			case TC_DEQUE:
				traverse(walker, container_cast<vector_t>(obj), depth, state, is_vector);
				break;
			case TC_ATTR_MAP:
				traverse(walker, container_cast<attr_map_t>(obj), depth, state, is_vector);
				break;
			case TC_PARAM_MAP:
				traverse(walker, container_cast<param_map_t>(obj), depth, state, is_vector);
				break;
			case TC_ATTR:
				traverse(walker, container_cast<attr_t>(obj), depth, state, is_vector);
				break;
			case TC_PARAM:
				traverse(walker, container_cast<param_t>(obj), depth, state, is_vector);
				break;
			default:
				walker.visit(obj, static_cast<type_code>(tag), depth, state, is_vector);
				break;
			}

		}
	}
	void traverse(object const& obj, dom_walker& walker) {
		traverse(walker, obj, 0, walker_state::LAST, false);
	}

}

