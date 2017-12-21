/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/dom/tree_walker.h>
#include <cyng/object_cast.hpp>
// #include <cyng/object.h>
#include <cyng/core/class_interface.h>
#include <cyng/intrinsics/traits/tag.hpp>

namespace cyng 
{
	//
	//	local function
	//
	void traverse(object const& node, tree_walker& walker, std::size_t depth)
	{
		//
		//	entry node
		//
		walker.current_node(depth, node);

		switch (node.get_class().tag())
		{
		case TC_TUPLE:
		{
			using type = typename std::tuple_element<type_code::TC_TUPLE, cyng::traits::tag_t>::type;
			const type* ptr = object_cast<type>(node);
			BOOST_ASSERT_MSG(ptr != nullptr, "invalid type info");
			for (auto obj : *ptr)
			{
				traverse(obj, walker, depth + 1);
			}
		}
		break;
		case TC_VECTOR:
		{
			using type = typename std::tuple_element<type_code::TC_VECTOR, cyng::traits::tag_t>::type;
			const type* ptr = object_cast<type>(node);
			BOOST_ASSERT_MSG(ptr != nullptr, "invalid type info");
			for (auto obj : *ptr)
			{
				traverse(obj, walker, depth + 1);
			}
		}
		break;
		case TC_SET:
		{
			using type = typename std::tuple_element<type_code::TC_SET, cyng::traits::tag_t>::type;
			const type* ptr = object_cast<type>(node);
			BOOST_ASSERT_MSG(ptr != nullptr, "invalid type info");
			for (auto obj : *ptr)
			{
				traverse(obj, walker, depth + 1);
			}
		}
		break;
		case TC_ATTR_MAP:
		{
			using type = typename std::tuple_element<type_code::TC_ATTR_MAP, cyng::traits::tag_t>::type;
			const type* ptr = object_cast<type>(node);
			BOOST_ASSERT_MSG(ptr != nullptr, "invalid type info");
			for (auto attr : *ptr)
			{
				traverse(attr.second, walker, depth + 1);
			}
		}
		break;
		case TC_PARAM_MAP:
		{
			using type = typename std::tuple_element<type_code::TC_PARAM_MAP, cyng::traits::tag_t>::type;
			const type* ptr = object_cast<type>(node);
			BOOST_ASSERT_MSG(ptr != nullptr, "invalid type info");
			for (auto param : *ptr)
			{
				traverse(param.second, walker, depth + 1);
			}
		}
		break;
		case TC_ATTR:
		{
			using type = typename std::tuple_element<type_code::TC_ATTR, cyng::traits::tag_t>::type;
			const type* ptr = object_cast<type>(node);
			BOOST_ASSERT_MSG(ptr != nullptr, "invalid type info");
			traverse(ptr->second, walker, depth + 1);
		}
		break;
		case TC_PARAM:
		{
			using type = typename std::tuple_element<type_code::TC_PARAM, cyng::traits::tag_t>::type;
			const type* ptr = object_cast<type>(node);
			BOOST_ASSERT_MSG(ptr != nullptr, "invalid type info");
			traverse(ptr->second, walker, depth + 1);
		}
		break;

		default:
			break;
		}

	}
	
	class dom_walker : public tree_walker
	{
	public:
		dom_walker(std::size_t& count)
		: count_(count)
		{}
		
		virtual bool current_node(std::size_t depth, object const&) override 
		{
			count_++;
			return true;
		}
		
		std::size_t& count_;
	};
	
	std::size_t dom_counter(object const& node)
	{
		std::size_t count{ 0 };
		dom_walker dw(count);
		traverse(node, dw, 0);
		return count;
	}
	
}




