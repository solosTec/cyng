#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/log/log.h>
#include <cyng/log/record.h>
#include <cyng/log/logger.h>

#include <cyng/task/controller.h>


BOOST_AUTO_TEST_SUITE(log_suite)

BOOST_AUTO_TEST_CASE(record)
{
	auto rec = cyng::logging::record::create(cyng::severity::LEVEL_TRACE, "hello");
#ifdef _DEBUG
	auto msg = cyng::to_str(*rec);
	std::cout << msg << std::endl;
#endif
}
BOOST_AUTO_TEST_CASE(logger)
{
	cyng::controller ctl;
	cyng::logger logger(ctl.create_channel<cyng::log>());

	auto rec = cyng::logging::record::create(cyng::severity::LEVEL_TRACE, "hello");
	rec << ", world";
	logger.push(*rec);

	//{
	//	using cyng::operator<<;
	//	auto __msg = cyng::logging::record::create(cyng::severity::LEVEL_INFO);
	//	__msg << 42;
	//	logger.push(*__msg);
	//}

	CYNG_LOG(logger, cyng::severity::LEVEL_INFO, 2);
	CYNG_LOG_ERROR(logger, "dude");

	ctl.get_registry().shutdown();
	ctl.stop();

}


BOOST_AUTO_TEST_SUITE_END()