#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/sys/host.h>
#include <cyng/sys/memory.h>
#include <cyng/sys/process.h>
#include <cyng/sys/cpu.h>
#include <cyng/sys/locale.h>
#include <cyng/sys/net.h>

#include <cyng/io/ostream.h>

#include <iostream>
#include <fstream> 
#include <thread>


BOOST_AUTO_TEST_SUITE(sys_suite)

BOOST_AUTO_TEST_CASE(host)
{
	// std::cout << cyng::sys::get_uptime().count() << std::endl;
	std::cout << cyng::to_string(cyng::sys::get_uptime()) << std::endl;
}
BOOST_AUTO_TEST_CASE(locale)
{
	// std::cout << cyng::sys::get_uptime().count() << std::endl;
	std::cout << cyng::sys::get_system_locale() << std::endl;
}

BOOST_AUTO_TEST_CASE(meminfo)
{
	//
	//	on the buildsystem should total RAM always the same.
	//
	auto const tr1 = cyng::sys::get_total_ram() / (1024 * 1024);
	auto const tr2 = cyng::sys::get_host_total_physical_memory();	//	MB
//	std::cout << tr1 << ", " << cyng::sys::get_host_total_physical_memory() << std::endl;	
	BOOST_CHECK_EQUAL(tr1, tr2);

	// std::cout << cyng::sys::get_total_ram() << std::endl;
	// std::cout << cyng::sys::get_used_ram() << std::endl;

	for(int i = 0; i < 100; ++i) {
		std::cout << cyng::sys::get_cpu_load(0) << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

BOOST_AUTO_TEST_CASE(net)
{
	//	138.201.95.180
	//std::cout << cyng::sys::resolve_address("segw.ch") << std::endl;
	BOOST_CHECK_EQUAL(cyng::sys::resolve_address("segw.ch").to_string(), "138.201.95.180");
	//cyng::sys::get_nic_names();
	//auto const pres = cyng::sys::get_nic_prefix();
	//for (auto const p : pres) {
	//	std::cout << p << std::endl;
	//}
	cyng::sys::get_address("Ethernet");
}

BOOST_AUTO_TEST_SUITE_END()