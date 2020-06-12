/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/dom/algorithm.h>
#include <cyng/object_cast.hpp>
#include <cyng/core/class_interface.h>
#include <cyng/intrinsics/traits/tag.hpp>
#include <cyng/factory.h>
#include <cyng/intrinsics/traits/tag.hpp>

#include <boost/algorithm/string.hpp>

namespace cyng 
{
	namespace 
	{
		
		/**
		 * Helper function to locate an attribute in a container class.
		 */
		template <typename T >
		auto lookup(T const& v, std::size_t idx) -> typename T::const_iterator
		{
			return std::find_if(v.begin(), v.end(), [idx](object const& obj) -> bool {
				return ((obj.get_class() == TC_ATTR) && (object_cast<attr_t>(obj)->first == idx));
			});
		}
		
		/**
		 * Helper function to locate a parameter in a container class.
		 * If container contains simple strings than parameter name is compared
		 * with this string. This is especially usefull for vectors of strings.
		 */
		template <typename T >
		auto lookup(T const& v, std::string const& name) -> typename T::const_iterator
		{
			return std::find_if(v.begin(), v.end(), [&name](object const& obj) -> bool {
				
// 				if (obj.get_class() == TC_PARAM)
// 				{
// 					std::cout 
// 					<< "lookup: " 
// 					<< name 
// 					<< " <=> "
// 					<< object_cast<param_t>(obj)->first
// 					<< std::endl;
// 				}
				
				return (((obj.get_class() == TC_PARAM) && (object_cast<param_t>(obj)->first == name))
				|| ((obj.get_class() == TC_STRING) && (*object_cast<std::string>(obj) == name)));
			});
		}
	}
	
	object find(vector_t const& v, std::size_t idx)
	{
		auto pos = lookup(v, idx);
		if (pos != v.end())
		{
			return object_cast<attr_t>(*pos)->second;
		}
		else if (idx < v.size())
		{
			return v[idx];
		}
		return make_object();
	}
	
	object find(tuple_t const& v, std::size_t idx)
	{
		auto pos = lookup(v, idx);
		if (pos != v.end())
		{
			return object_cast<attr_t>(*pos)->second;
		}
		else if (idx < v.size())
		{
			pos = v.begin();
			std::advance(pos, idx);
			return *pos;
		}
		return make_object();
	}
	
	object find(set_t const& v, std::size_t idx)
	{
		auto pos = lookup(v, idx);
		return (pos != v.end())
			? object_cast<attr_t>(*pos)->second
			: make_object()
			;
	}
	
	object find(attr_map_t const& v, std::size_t idx)
	{
		auto pos = v.find(idx);
		return (pos != v.end())
			? pos->second
			: make_object()
			;
	}

	object find(attr_map_t const&, std::string const&)
	{
		return make_object();
	}

	
	object find(attr_t const& attr, std::size_t idx)
	{
		return (attr.first == idx)
			? attr.second
			: make_object()
			;
	}
	
	object find(object const& obj, std::size_t idx)
	{
		switch (obj.get_class().tag())
		{
			case TC_TUPLE:
				return find(*object_cast<tuple_t>(obj), idx);
			case TC_VECTOR:
				return find(*object_cast<vector_t>(obj), idx);
			case TC_SET:
				return find(*object_cast<set_t>(obj), idx);
			case TC_ATTR_MAP:
				return find(*object_cast<attr_map_t>(obj), idx);
			case TC_ATTR:
				return find(*object_cast<attr_t>(obj), idx);
			case TC_PARAM_MAP:
			case TC_PARAM:
			default:
				break;
		}
		return obj;
	}
	
	object find(vector_t const& v, std::string const& name)
	{
		auto pos = lookup(v, name);
		if (pos != v.end())
		{
			//
			//	unpack parameter value
			//
			if ((*pos).get_class() == TC_PARAM)
			{
				return object_cast<param_t>(*pos)->second;
			}

			//
			//	It seems that is doesn't make much sense to return the string that we were looking for.
			//	But to return a boolean value could be confusing too. So we return the value that
			//	was found. To indicate an successful search the safe bool conversion of object could
			//	be usefull
			//
			return *pos;
		}
		return make_object();
	}
	
	object find(tuple_t const& v, std::string const& name)
	{
		auto pos = lookup(v, name);
		return (pos != v.end())
			? object_cast<param_t>(*pos)->second
			: make_object()
			;
	}
	
	object find(set_t const& v, std::string const& name)
	{
		auto pos = lookup(v, name);
		return (pos != v.end())
			? object_cast<param_t>(*pos)->second
			: make_object()
			;
	}
	
	object find(param_map_t const& v, std::string const& name)
	{
		auto pos = v.find(name);
		return (pos != v.end())
			? pos->second
			: make_object()
			;
	}
	
	object find(param_map_t const& v, std::size_t idx)
	{
		return find(v, std::to_string(idx));

	}
	object find(param_t const& param, std::string const& name)
	{
		return (param.first == name)
			? param.second
			: make_object()
			;
	}

	object find(object const& obj, std::string const& name)
	{
		switch (obj.get_class().tag())
		{
			case TC_TUPLE:
				return find(*object_cast<tuple_t>(obj), name);
			case TC_VECTOR:
				return find(*object_cast<vector_t>(obj), name);
			case TC_SET:
				return find(*object_cast<set_t>(obj), name);
			case TC_PARAM_MAP:
				return find(*object_cast<param_map_t>(obj), name);
			case TC_PARAM:
				return find(*object_cast<param_t>(obj), name);
			case TC_ATTR_MAP:
			case TC_ATTR:
			default:
				break;
		}
		return obj;
	}

	bool exists(attr_map_t const&, std::string const& name)
	{
		return false;
	}

	bool exists(param_map_t const& pm, std::string const& name)
	{
		return pm.find(name) != pm.end();
	}

	bool exists(param_t const& param, std::string const& name)
	{
		return boost::algorithm::equals(param.first, name);
	}

	bool exists(set_t const& set, std::string const& name)
	{
		return lookup(set, name) != set.end();
	}

	bool exists(vector_t const& vec, std::string const& name)
	{
		return lookup(vec, name) != vec.end();
	}

	bool exists(tuple_t const& tpl, std::string const& name)
	{
		return lookup(tpl, name) != tpl.end();
	}

	bool exists(object const& obj, std::string const& name)
	{
		switch (obj.get_class().tag())
		{
		case TC_TUPLE:
			return exists(*object_cast<tuple_t>(obj), name);
		case TC_VECTOR:
			return exists(*object_cast<vector_t>(obj), name);
		case TC_SET:
			return exists(*object_cast<set_t>(obj), name);
		case TC_PARAM_MAP:
			return exists(*object_cast<param_map_t>(obj), name);
		case TC_PARAM:
			return exists(*object_cast<param_t>(obj), name);
		case TC_ATTR_MAP:
		case TC_ATTR:
		default:
			break;
		}
		return false;
	}

	
}
