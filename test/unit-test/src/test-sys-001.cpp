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
#include <cyng/sys/mac.h>
#include <cyng/sys/ntp.h>
#include <cyng/io/serializer.h>

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

        auto const macs = sys::retrieve_mac48();
#ifdef _DEBUG
        for (auto const& m : macs) {
            using namespace io;
            std::cout << m << std::endl;
        }
#endif

		auto srvs =	sys::get_ntp_servers();
#ifdef _DEBUG
		for (auto const& srv : srvs) {
			std::cout << srv << std::endl;
		}
#endif

		return true;
	}
	
}
