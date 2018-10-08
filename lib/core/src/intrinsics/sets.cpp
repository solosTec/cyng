/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/intrinsics/sets.h>
#include <cyng/object.h>

// template struct std::list<cyng::object>;
template class std::vector<cyng::object>;
template class std::set<cyng::object, std::less<cyng::object>>;

template class std::map<std::size_t, cyng::object>;
template struct std::pair<std::size_t, cyng::object>;

template class std::map<std::string, cyng::object>;
template struct std::pair<std::string, cyng::object>;

#include <algorithm>

namespace std
{
	bool less<cyng::tuple_t>::operator()(cyng::tuple_t const& c1, cyng::tuple_t const& c2) const noexcept {
		return lexicographical_compare(c1.begin(), c1.end(), c2.begin(), c2.end(), [](cyng::object const& o1, cyng::object const& o2) {
			return cyng::less_by_value(o1, o2);
		});
	}

	bool less<cyng::vector_t>::operator()(cyng::vector_t const& c1, cyng::vector_t const& c2) const noexcept {
		return lexicographical_compare(c1.begin(), c1.end(), c2.begin(), c2.end(), [](cyng::object const& o1, cyng::object const& o2) {
			return cyng::less_by_value(o1, o2);
		});
	}

}