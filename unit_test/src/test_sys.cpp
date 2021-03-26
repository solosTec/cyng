#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/sys/host.h>
#include <cyng/sys/memory.h>
#include <cyng/sys/process.h>
#include <cyng/sys/locale.h>
#include <cyng/io/ostream.h>

#include <iostream>
#include <fstream> 


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
	//std::ifstream ifs("D:\\reboot\\cyng\\unit_test\\src\\meminfo.txt");
	//std::string name, unit;
	//std::uint64_t value;
	//while (ifs) {
	//	ifs >> name >> value >> unit;
	//}

}


BOOST_AUTO_TEST_SUITE_END()