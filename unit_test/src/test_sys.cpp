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
#include <cyng/sys/clock.h>


#include <cyng/io/ostream.h>

#include <iostream>
#include <fstream> 
#include <thread>
#include <ctime>

#include <boost/locale.hpp>


BOOST_AUTO_TEST_SUITE(sys_suite)

BOOST_AUTO_TEST_CASE(host)
{
	// std::cout << cyng::sys::get_uptime().count() << std::endl;
	std::cout << cyng::to_string(cyng::sys::get_uptime()) << std::endl;
}
BOOST_AUTO_TEST_CASE(locale)
{
	//using namespace boost::locale;
	//using namespace std;
	boost::locale::generator gen;
	auto loc = gen("");
	// Create system default locale
	std::locale::global(loc);
	// Make it system global

	std::cout.imbue(loc);
	// Set as default locale for output

	std::cout << boost::locale::format("Today {1,date} at {1,time} we had run our first localization example") % time(0)
		<< std::endl;

	std::cout << "This is how we show numbers in this locale " << boost::locale::as::number << 103.34 << std::endl;
	std::cout << "This is how we show currency in this locale " << boost::locale::as::currency << 103.34 << std::endl;
	std::cout << "This is typical date in the locale " << boost::locale::as::date << std::time(0) << std::endl;
	std::cout << "This is typical time in the locale " << boost::locale::as::time << std::time(0) << std::endl;
	std::cout << "This is upper case " << boost::locale::to_upper("Hello World!") << std::endl;
	std::cout << "This is lower case " << boost::locale::to_lower("Hello World!") << std::endl;
	std::cout << "This is title case " << boost::locale::to_title("Hello World!") << std::endl;
	std::cout << "This is fold case " << boost::locale::fold_case("Hello World!") << std::endl;

	//std::cout << loc.name() << std::endl;
	std::cout << std::use_facet<boost::locale::info>(loc).name() << std::endl;
	std::cout << std::use_facet<boost::locale::info>(loc).country() << std::endl;
	std::cout << std::use_facet<boost::locale::info>(loc).language() << std::endl;
	//std::cout << std::use_facet<boost::locale::info>(loc).variant() << std::endl;
	std::cout << std::use_facet<boost::locale::info>(loc).encoding() << std::endl;
	// std::cout << cyng::sys::get_uptime().count() << std::endl;
	std::cout << cyng::sys::get_system_locale().at(0) << std::endl;
}

BOOST_AUTO_TEST_CASE(meminfo)
{
	//
	//	on the buildsystem should the total RAM always be the same.
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

	for(int i = 12; i != 0; --i) {
		//std::cout << '#' << i << " - "cyng::sys::get_cpu_load(0) << std::endl;
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
	auto const pres = cyng::sys::get_nic_names();
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

BOOST_AUTO_TEST_CASE(clock)
{
	// 2022-07-10 16:56:32
	auto const now = std::chrono::time_point<std::chrono::system_clock>(std::chrono::seconds(1657472192));

	auto const v0 = cyng::sys::get_start_of_day(now);
	//	2022-07-10 00:00:00.0000000
	std::cout << v0 << std::endl;

	auto const v1 = cyng::sys::get_start_of_month(now);
	//	2022-07-01 00:00:00.0000000
	std::cout << v1 << std::endl;

	auto const v2 = cyng::sys::get_end_of_month(now);
	//	2022-07-31 00:00:00.0000000
	std::cout << v2 << std::endl;

	//auto const v3 = cyng::sys::get_iso_week_number(now);
	////	27
	//std::cout << v3 << std::endl;
	//BOOST_REQUIRE_EQUAL(v3, 27);

	auto const v4 = cyng::sys::get_length_of_month(now);
	//	744h
	std::cout << v4 << std::endl;
	BOOST_REQUIRE_EQUAL(v4.count(), 31 * 24);

	auto const v5 = cyng::sys::get_end_of_year(now);
	//	2022-12-31 00:00:00.0000000
	std::cout << v5 << std::endl;

	auto const v6 = cyng::sys::get_start_of_year(now);
	//	2022-01-01 00:00:00.0000000
	std::cout << v6 << std::endl;

	auto const v7 = cyng::sys::get_length_of_year(now);
	//	8736h (364d)
	std::cout << v7 << std::endl;
	BOOST_REQUIRE_EQUAL(v7.count(), 364 * 24);

	//auto const v8 = cyng::sys::get_day_of_week(now);
	////	7 (= Sunday)
	//std::cout << v8 << std::endl;
	//BOOST_REQUIRE_EQUAL(v8, 7);
}

BOOST_AUTO_TEST_SUITE_END()