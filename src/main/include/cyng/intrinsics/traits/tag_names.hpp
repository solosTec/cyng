/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_TRAITS_TAG_NAMES_HPP
#define CYNG_TRAITS_TAG_NAMES_HPP


#include <cyng/intrinsics/traits/tag.hpp>
#include <cyng/intrinsics/traits.hpp>

namespace cyng 
{
	namespace traits
	{
		const char* get_type_name(std::size_t);
		
        template <typename T>
        constexpr typename std::enable_if<has_type<T, tag_t>::value, const char*>::type
        get_tag_name()
        {
            return get_type_name(type_tag_traits<T>());
        }
		
        template <typename T>
        constexpr typename std::enable_if<!has_type<T, tag_t>::value, const char*>::type
        get_tag_name()
        {
            return cyng::traits::type_tag<T>::name;
        }
	}
}

#endif // CYNG_TRAITS_TAG_NAMES_HPP
