#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/sql/sql.hpp>
#include <cyng/sql/dsl.h>
#include <cyng/sql/dsl/placeholder.h>

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

	cyng::sql::select s(cyng::sql::dialect::SQLITE, ms);
	std::cout << s.all(ms, true).from("T")()  << std::endl;
	std::cout << s.sum(ms, 3).from()() << std::endl;

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
	//std::cout << cyng::sql::create(cyng::sql::dialect::POSTGRESQL, ms).to_string() << std::endl;
	auto const sql1 = cyng::sql::create(cyng::sql::dialect::POSTGRESQL, ms).to_string();
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
	//auto const sql1 = cyng::sql::insert(cyng::sql::dialect::SQLITE, ms).to_string();

	auto const sql = cyng::sql::insert(cyng::sql::dialect::SQLITE, ms).bind_values(ms)();

	//	INSERT INTO TDemo( id, gen, name, age, tag ) VALUES( ?, ?, ?, julianday(?), ? )
	BOOST_REQUIRE_EQUAL(sql, "INSERT INTO TDemo( id, gen, name, age, tag ) VALUES( ?, ?, ?, julianday(?), ? )");

	//	INSERT INTO TDemo( id, gen, name, age, tag ) VALUES( 1, 2, 'three', datetime(2021-02-15T20:24:09+0100), 5 )
	std::cout << cyng::sql::insert(cyng::sql::dialect::SQLITE, ms).values(ms, 1, 2, "three", std::chrono::system_clock::now(), 5)() << std::endl;

}

BOOST_AUTO_TEST_CASE(update)
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
	//	set placeholder for all values
	{
		// UPDATE TDemo SET gen = ?, name = ?, age = julianday(?), tag = ? WHERE id = ?
		auto const sql = cyng::sql::update(cyng::sql::dialect::SQLITE, ms).set_placeholder().where(cyng::sql::pk())();
		//std::cout << sql << std::endl;
		BOOST_CHECK_EQUAL(sql, "UPDATE TDemo SET gen = ?, name = ?, age = julianday(?), tag = ? WHERE id = ?");
	}
	std::cout << cyng::sql::update(cyng::sql::dialect::SQLITE, ms).set_placeholder().where(cyng::sql::make_constant("age") > cyng::sql::placeholder("age"))() << std::endl;
}

BOOST_AUTO_TEST_CASE(remove)
{
	cyng::meta_store const m("demo"
		, {
			cyng::column("id", cyng::TC_INT64),
			cyng::column("name", cyng::TC_STRING),
			cyng::column("age", cyng::TC_TIME_POINT),
			cyng::column("tag", cyng::TC_UUID)
		}
	, 2);

	auto const ms = cyng::to_sql(m, { 0, 65, 32, 64 });
	std::cout << cyng::sql::remove(cyng::sql::dialect::SQLITE, ms)() << std::endl;
	std::cout << cyng::sql::remove(cyng::sql::dialect::SQLITE, ms).where(cyng::sql::pk{})() << std::endl;
}

BOOST_AUTO_TEST_CASE(expr)
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
	//std::cout << cyng::sql::remove(cyng::sql::dialect::SQLITE, ms)() << std::endl;
	using namespace cyng::sql;
	cyng::sql::constant<const char[6]> s("hello");
	cyng::sql::constant<const char*> s2("hello");
	auto s3 = cyng::sql::make_constant("hello");	// const char[6]
	//cyng::sql::constant s4("hello");	// ?
	std::cout << cyng::sql::remove(cyng::sql::dialect::SQLITE, ms).where(cyng::sql::constant(std::chrono::system_clock::now()) > cyng::sql::constant(2))() << std::endl;

}

BOOST_AUTO_TEST_SUITE_END()