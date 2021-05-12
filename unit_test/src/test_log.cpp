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
	cyng::controller ctl(4);
	cyng::logger logger(ctl.create_channel<cyng::log>());

	logger.start_console_logger();
	logger.start_file_logger(std::filesystem::temp_directory_path() / "docc.log", 32UL * 1024UL * 1024UL);

	auto rec = cyng::logging::record::create(cyng::severity::LEVEL_TRACE, "hello");
	rec << ", world";
	logger.push(*rec);


	CYNG_LOG(logger, cyng::severity::LEVEL_INFO, 2);
	CYNG_LOG_ERROR(logger, "dude");

	std::this_thread::sleep_for(std::chrono::seconds(2));

	ctl.get_registry().shutdown();
	ctl.shutdown();
	ctl.stop();

}


BOOST_AUTO_TEST_SUITE_END()