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
#include <cyng/sys/port.h>
#include <cyng/io/serializer.h>

#if BOOST_OS_LINUX
#include <cyng/sys/rtc.h>
#endif

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

		auto ports = sys::get_ports();
#ifdef _DEBUG
		for (auto const& port : ports) {
			std::cout << port << std::endl;
		}
#endif

#if BOOST_OS_LINUX
        // CAP_SYS_TIME privilege required.
        auto const ts = sys::read_hw_clock(0);
        BOOST_CHECK(ts.second);
#ifdef _DEBUG
		std::cout << to_str(ts.first) << std::endl;
#endif
#endif

		return true;
	}
	
}
