#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/db/session.h>
#include <cyng/io/ostream.h>

#include <cyng/obj/util.hpp>

#include <iostream>

BOOST_AUTO_TEST_SUITE(db_suite)

BOOST_AUTO_TEST_CASE(session)
{
	cyng::db::session	s(cyng::db::connection_type::CONNECTION_SQLITE);
	s.connect(cyng::make_tuple(
		cyng::make_param("type", "SQLite"), 
		cyng::make_param("file-name", "demo.db"),
		cyng::make_param("busy-timeout", 3800)
	));
	BOOST_REQUIRE(s.is_alive());
	auto const d = s.get_dialect();
	BOOST_REQUIRE(d == cyng::sql::dialect::SQLITE);
	//s.execute("create table DEMO (id int)");
	s.close();
	BOOST_REQUIRE(!s.is_alive());
}
BOOST_AUTO_TEST_CASE(SQLite)
{
	
}

BOOST_AUTO_TEST_CASE(ODBC)
{

}

BOOST_AUTO_TEST_SUITE_END()