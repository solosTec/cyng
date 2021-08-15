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
#include <cyng/parse/net.h>


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
}

BOOST_AUTO_TEST_CASE(cpu)
{

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

	auto const cfgv4 = cyng::sys::get_ipv4_configuration();
	for (auto const c : cfgv4) {
		std::cout << c << std::endl;
	}

	auto const cfg_filtered = cyng::sys::filter(cfgv4, cyng::sys::filter_by_name("Ethernet"));
	for (auto const c : cfg_filtered) {
		std::cout << c << std::endl;
	}

	auto const cfgv6 = cyng::sys::get_ipv6_configuration();
	for (auto const c : cfgv6) {
		std::cout << c << std::endl;
	}

	//cyng::sys::get_nic_names();
	auto const pres = cyng::sys::get_nic_prefix();
	for (auto const p : pres) {
		std::cout << p << std::endl;
	}
	//cyng::sys::get_address("Ethernet");
	//std::cout << cyng::sys::get_address("ens33");

	auto const addr = cyng::sys::get_address_IPv6("Ethernet");

#if defined(BOOST_OS_LINUX_AVAILABLE)
	// cyng::sys::read_ipv6_info([](std::string address, std::string name, std::uint64_t index, std::uint64_t len, std::uint64_t scope, std::uint64_t flag) -> bool {
	// 	std::cout << address << " - " << name << " - " << scope << std::endl;
	// 	if (boost::algorithm::equals(name, "ens33") && 0x020 == scope) {
	// 		std::cout << cyng::to_ipv6(address, 0) << "%" << name << std::endl;
	// 		return false;
	// 	}
	// 	return true;
	// 	});
#endif
}

BOOST_AUTO_TEST_SUITE_END()