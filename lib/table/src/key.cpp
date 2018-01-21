/*
* The MIT License (MIT)
*
* Copyright (c) 2017 Sylko Olzscher
*
*/

#include <cyng/table/key.hpp>

namespace cyng
{
	namespace store
	{
// 		key::key()
// 			: pk_()
// 			, generation_(0)
// 		{}
	}

	namespace traits
	{

// #if defined(CYNG_LEGACY_MODE_ON)
// 		const char type_tag<cyng::store::key>::name[] = "key";
// #endif
	}	// traits	

}

namespace std
{
// 	size_t hash<cyng::store::key>::operator()(cyng::store::key pk) const noexcept
// 	{
// 		return hash<cyng::table::key_type>()(pk.pk_);
// 	}
// 
// 	bool equal_to<cyng::store::key>::operator()(cyng::store::key const& c1, cyng::store::key const& c2) const noexcept
// 	{
// 		return c1.pk_ == c2.pk_;
// 	}

}

