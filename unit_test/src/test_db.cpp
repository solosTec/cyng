#ifdef STAND_ALONE
#define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/db/session.h>
#include <cyng/db/storage.h>
#include <cyng/io/ostream.h>
#include <cyng/obj/tag.hpp>

#include <cyng/db/details/statement_interface.h>
#include <cyng/obj/util.hpp>
#include <cyng/sys/clock.h>

#include <iostream>

BOOST_AUTO_TEST_SUITE(db_suite)

BOOST_AUTO_TEST_CASE(session) {
    cyng::db::session s(cyng::db::connection_type::SQLITE);
    s.connect(cyng::make_tuple(
        cyng::make_param("type", "SQLite"), cyng::make_param("file.name", "demo.db"), cyng::make_param("busy.timeout", 3800)));
    BOOST_REQUIRE(s.is_alive());
    auto const d = s.get_dialect();
    BOOST_REQUIRE(d == cyng::sql::dialect::SQLITE);
    // s.execute("create table DEMO (id int)");
    s.close();
    BOOST_REQUIRE(!s.is_alive());
}
BOOST_AUTO_TEST_CASE(SQLite) {
    cyng::db::session s(cyng::db::connection_type::SQLITE);
    s.connect(cyng::make_tuple(
        cyng::make_param("type", "SQLite"), cyng::make_param("file.name", "demo.db"), cyng::make_param("busy.timeout", 3800)));
    BOOST_REQUIRE(s.is_alive());
    auto const d = s.get_dialect();
    BOOST_REQUIRE(d == cyng::sql::dialect::SQLITE);
    s.execute("DROP TABLE demo");
    s.execute("CREATE TABLE demo (id int, age float)");

    //
    //  generate a time stamp
    //
    std::tm tm = {};
    std::istringstream ss("2011-Februar-18 23:12:34");
    ss.imbue(std::locale("de_DE.utf-8"));
    ss >> std::get_time(&tm, "%Y-%b-%d %H:%M:%S");

    auto const t = std::mktime(&tm);
    auto const tp = std::chrono::system_clock::from_time_t(t);

    std::cout << cyng::sys::to_string(tp, "%F %T%z") << std::endl;
    std::cout << cyng::sys::to_string_utc(tp, "%F %T%z (UTC)") << std::endl;

    //
    //  store into database
    //
    {
        auto stmt = s.create_statement();
        auto const r = stmt->prepare("INSERT INTO demo VALUES (?, julianday(?))");
        if (r.second) {
            stmt->push(cyng::make_object(1), 0);  //	id
            stmt->push(cyng::make_object(tp), 0); //	age => 1|2011-02-18 22:12:34
            if (stmt->execute()) {
                stmt->clear();
            }
        }
    }

    //
    //  read from database
    //
    {
        // auto const ms = config::get_table_sml_readout();
        std::string const sql = "SELECT datetime(age) "
                                "FROM demo "
                                "WHERE id = ? ";
        auto stmt = s.create_statement();
        std::pair<int, bool> const r = stmt->prepare(sql);
        if (r.second) {
            stmt->push(cyng::make_object(1), 0); //	id

            while (auto res = stmt->get_result()) {
                auto obj = res->get(1, cyng::TC_TIME_POINT, 0);
                auto const tpr = cyng::value_cast(obj, std::chrono::system_clock::now());
                std::cout << cyng::sys::to_string(tpr, "%F %T%z") << std::endl;
                std::cout << cyng::sys::to_string_utc(tpr, "%F %T%z (UTC)") << std::endl;
            }
        }
    }

    s.close();
    BOOST_REQUIRE(!s.is_alive());
}

BOOST_AUTO_TEST_CASE(ODBC) {}

BOOST_AUTO_TEST_SUITE_END()
