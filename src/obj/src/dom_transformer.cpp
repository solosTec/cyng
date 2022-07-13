/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/obj/algorithm/dom_transformer.h>
#include <cyng/obj/container_cast.hpp>

namespace cyng {

	namespace {
		using fun_t = std::function<object(object&, std::size_t, type_code)>;
		void transform(object& obj, std::size_t depth, type_code tag, fun_t f);
		void transform(tuple_t* tpl, std::size_t depth, fun_t f);
		void transform(vector_t* vec, std::size_t depth, fun_t f);
		void transform(deque_t* deq, std::size_t depth, fun_t f);
		void transform(attr_map_t* amap, std::size_t depth, fun_t f);
		void transform(param_map_t* pmap, std::size_t depth, fun_t f);
		void transform(attr_t* attr, std::size_t depth, fun_t f);
		void transform(param_t* param, std::size_t depth, fun_t f);

		void transform(tuple_t* tpl, std::size_t depth, fun_t f) {
			for (auto& obj : *tpl) {
				auto const tag = obj.rtti().tag();
				transform(obj, depth + 1, static_cast<type_code>(tag), f);
			}
		}

		void transform(vector_t* vec, std::size_t depth, fun_t f) {
			for (auto& obj : *vec) {
				auto const tag = obj.rtti().tag();
				transform(obj, depth + 1, static_cast<type_code>(tag), f);
			}
		}

		void transform(deque_t* deq, std::size_t depth, fun_t f) {
			for (auto& obj : *deq) {
				auto const tag = obj.rtti().tag();
				transform(obj, depth + 1, static_cast<type_code>(tag), f);
			}
		}

		void transform(attr_map_t* amap, std::size_t depth, fun_t f) {
			for (auto & attr : *amap) {
				auto const tag = attr.second.rtti().tag();
				transform(attr.second, depth + 1, static_cast<type_code>(tag), f);
			}
		}

		void transform(param_map_t* pmap, std::size_t depth, fun_t f) {
			for (auto & param : *pmap) {
				auto const tag = param.second.rtti().tag();
				transform(param.second, depth + 1, static_cast<type_code>(tag), f);
			}
		}

		void transform(attr_t* attr, std::size_t depth, fun_t f) {
			auto const tag = attr->second.rtti().tag();
			transform(attr->second, depth, static_cast<type_code>(tag), f);
		}

		void transform(param_t* param, std::size_t depth, fun_t f) {
			auto const tag = param->second.rtti().tag();
			transform(param->second, depth, static_cast<type_code>(tag), f);
		}

		void transform(object& obj, std::size_t depth, type_code tag, fun_t f) {

			switch (tag) {
			case TC_TUPLE:
				transform(object_cast<tuple_t>(obj), depth, f);
				break;
			case TC_VECTOR:
				transform(object_cast<vector_t>(obj), depth, f);
				break;
			case TC_DEQUE:
				transform(object_cast<vector_t>(obj), depth, f);
				break;
			case TC_ATTR_MAP:
				transform(object_cast<attr_map_t>(obj), depth, f);
				break;
			case TC_PARAM_MAP:
				transform(object_cast<param_map_t>(obj), depth, f);
				break;
			case TC_ATTR:
				transform(object_cast<attr_t>(obj), depth, f);
				break;
			case TC_PARAM:
				transform(object_cast<param_t>(obj), depth, f);
				break;
			default:
				obj = f(obj, depth, tag);
				break;
			}
		}
	}
	void transform(object& obj, std::function<object(object const&, std::size_t, type_code)> f) {
		auto const tag = obj.rtti().tag();
		transform(obj, 0, static_cast<type_code>(tag), f);
	}
}

