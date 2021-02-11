#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/sql/sql.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(sql_suite)

BOOST_AUTO_TEST_CASE(select)
{
	cyng::meta_store const m("demo"
		, {
			cyng::column("id", cyng::TC_INT64),
			cyng::column("name", cyng::TC_STRING),
			cyng::column("age", cyng::TC_TIME_POINT),
			cyng::column("tag", cyng::TC_UUID)
		}
	, 1);

	auto const ms = cyng::to_sql(m, {0, 64, 32, 64});

	cyng::sql::select s(cyng::sql::dialect::SQLITE);
	std::cout << s.all(ms, true).from("T")()  << std::endl;
	std::cout << s.sum(ms, 3).from(ms)() << std::endl;

}
BOOST_AUTO_TEST_CASE(create)
{
	cyng::meta_store const m("demo"
		, {
			cyng::column("id", cyng::TC_INT64),
			cyng::column("name", cyng::TC_STRING),
			cyng::column("age", cyng::TC_TIME_POINT),
			cyng::column("tag", cyng::TC_UUID)
		}
	, 1);

	auto const ms = cyng::to_sql(m, { 0, 65, 32, 64 });
	//std::cout << cyng::sql::create(cyng::sql::dialect::POSTGRESQL, ms).to_str() << std::endl;
	auto const sql1 = cyng::sql::create(cyng::sql::dialect::POSTGRESQL, ms).to_str();
	//	CREATE TABLE TDemo IF NOT EXISTS( id bigint, gen bigint, name VARCHAR(65), age timestamp, tag uuid, PRIMARY KEY( id ))
	BOOST_REQUIRE_EQUAL(sql1, "CREATE TABLE TDemo IF NOT EXISTS( id bigint, gen bigint, name VARCHAR(65), age timestamp, tag uuid, PRIMARY KEY( id ))");

}

BOOST_AUTO_TEST_CASE(insert)
{
	cyng::meta_store const m("demo"
		, {
			cyng::column("id", cyng::TC_INT64),
			cyng::column("name", cyng::TC_STRING),
			cyng::column("age", cyng::TC_TIME_POINT),
			cyng::column("tag", cyng::TC_UUID) 
		}
	, 1);

	auto const ms = cyng::to_sql(m, { 0, 65, 32, 64 });
	std::cout << cyng::sql::insert(cyng::sql::dialect::SQLITE, ms).to_str() << std::endl;
	auto const sql1 = cyng::sql::insert(cyng::sql::dialect::SQLITE, ms).to_str();
	//	CREATE TABLE TDemo IF NOT EXISTS( id bigint, gen bigint, name VARCHAR(65), age timestamp, tag uuid, PRIMARY KEY( id ))
	//BOOST_REQUIRE_EQUAL(sql1, "CREATE TABLE TDemo IF NOT EXISTS( id bigint, gen bigint, name VARCHAR(65), age timestamp, tag uuid, PRIMARY KEY( id ))");

}


BOOST_AUTO_TEST_SUITE_END()