/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/intrinsics/sets.h>
#include <cyng/object.h>

// template struct std::list<cyng::object>;
template struct std::vector<cyng::object>;
template struct std::set<cyng::object, std::less<cyng::object>>;

template struct std::map<std::size_t, cyng::object>;
template struct std::pair<std::size_t, cyng::object>;

template struct std::map<std::string, cyng::object>;
template struct std::pair<std::string, cyng::object>;