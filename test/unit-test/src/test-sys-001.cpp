/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-sys-001.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/sys/fsys.h>

namespace cyng 
{
	bool test_sys_001()
	{
		auto drives = sys::get_drive_names();
#ifdef _DEBUG
		for (auto const& d : drives) {
			std::cout << d << std::endl;
		}
#endif
		BOOST_CHECK(!drives.empty());
		return true;
	}
	
}
