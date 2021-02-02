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
	auto rec = docscript::logging::record::create(docscript::severity::LEVEL_TRACE, "hello");
#ifdef _DEBUG
	auto msg = docscript::to_str(*rec);
	std::cout << msg << std::endl;
#endif
}
BOOST_AUTO_TEST_CASE(logger)
{
	docscript::controller ctl;
	docscript::logger logger(ctl.create_channel<docscript::log>());

	auto rec = docscript::logging::record::create(docscript::severity::LEVEL_TRACE, "hello");
	rec << ", world";
	logger.push(*rec);

	//{
	//	using docscript::operator<<;
	//	auto __msg = docscript::logging::record::create(docscript::severity::LEVEL_INFO);
	//	__msg << 42;
	//	logger.push(*__msg);
	//}

	CYNG_LOG(logger, docscript::severity::LEVEL_INFO, 2);
	CYNG_LOG_ERROR(logger, "dude");

	ctl.get_registry().shutdown();
	ctl.stop();

}


BOOST_AUTO_TEST_SUITE_END()