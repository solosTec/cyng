#ifdef STAND_ALONE
#define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/store/auto_table.h>
#include <cyng/store/db.h>
#include <cyng/store/meta.h>
#include <cyng/store/slot.h>

#include <cyng/io/ostream.h>
#include <cyng/obj/tag.hpp>
#include <cyng/obj/value_cast.hpp>
#include <cyng/parse/string.h>

#include <boost/uuid/nil_generator.hpp>

BOOST_AUTO_TEST_SUITE(store_suite)

BOOST_AUTO_TEST_CASE(meta) {
    cyng::meta_store const m(
        "demo",
        {cyng::column("id", cyng::TC_INT64), cyng::column("name", cyng::TC_STRING), cyng::column("age", cyng::TC_TIME_POINT)},
        1);
    // std::cerr << m.get_index_by_name("id") << std::endl;
    BOOST_REQUIRE_EQUAL(m.get_index_by_name("id"), 0);
    BOOST_REQUIRE_EQUAL(m.get_index_by_name("name"), 1);
    BOOST_REQUIRE_EQUAL(m.get_index_by_name("age"), 2);
    BOOST_REQUIRE(m.is_pk("id"));
    BOOST_REQUIRE(!m.is_pk("name"));
    BOOST_REQUIRE(!m.is_pk("age"));
}

BOOST_AUTO_TEST_CASE(table) {
    cyng::meta_store const m(
        "demo",
        {cyng::column("id", cyng::TC_INT64), cyng::column("name", cyng::TC_STRING), cyng::column("age", cyng::TC_TIME_POINT)},
        1);

    cyng::table tbl(m);
    auto const key = cyng::key_generator(12u);
    tbl.insert(
        key,
        cyng::data_generator("A", std::chrono::system_clock::now()),
        1u //	gen
        ,
        boost::uuids::nil_uuid());

    // std::cout << tbl.size() << std::endl;
    BOOST_REQUIRE_EQUAL(tbl.size(), 1);
    BOOST_REQUIRE(tbl.exists(key));

    tbl.insert(
        cyng::key_generator(42u),
        cyng::data_generator("O", std::chrono::system_clock::now()),
        1u //	gen
        ,
        boost::uuids::nil_uuid());

    BOOST_REQUIRE_EQUAL(tbl.size(), 2);

    auto const rec = tbl.lookup(key);
    auto tpl = rec.to_tuple();
    // std::cout << tpl << std::endl;

    auto rec2 = tbl.find_first("name", std::string("O"));
    // std::cout << rec2.to_tuple() << std::endl;
}

BOOST_AUTO_TEST_CASE(erase) {
    cyng::meta_store const m(
        "demo",
        {cyng::column("tag", cyng::TC_UUID), cyng::column("name", cyng::TC_STRING), cyng::column("age", cyng::TC_TIME_POINT)},
        1);

    cyng::table tbl(m);
    auto const tag = cyng::to_uuid("28c4b783-f35d-49f1-9027-a75dbae9f5e2");
    auto const key = cyng::key_generator(tag);
    tbl.insert(
        key,
        cyng::data_generator("A", std::chrono::system_clock::now()),
        1u //	gen
        ,
        boost::uuids::nil_uuid());

    // std::cout << tbl.size() << std::endl;
    BOOST_REQUIRE_EQUAL(tbl.size(), 1);
    BOOST_REQUIRE(tbl.exists(key));

    auto const b = tbl.erase(key, boost::uuids::nil_uuid());
    BOOST_REQUIRE(b);
    BOOST_REQUIRE_EQUAL(tbl.size(), 0);
}

BOOST_AUTO_TEST_CASE(auto_table) {
    cyng::meta_store const m(
        "demo",
        {cyng::column("id", cyng::TC_INT64), cyng::column("name", cyng::TC_STRING), cyng::column("age", cyng::TC_TIME_POINT)},
        1);

    auto const key = cyng::key_generator<std::uint64_t>(2u);

    cyng::auto_table tbl(m, key, [](cyng::table *tbl, cyng::key_t const &key) {
        BOOST_CHECK_EQUAL(key.size(), 1);
        auto tmp = cyng::value_cast<std::uint64_t>(key.at(0), 0);
        BOOST_REQUIRE_EQUAL(tmp, 2);
        return cyng::key_generator(tmp + 1);
    });

    BOOST_REQUIRE(tbl.is_auto());

    tbl.insert(cyng::data_generator("Q", std::chrono::system_clock::now()), boost::uuids::nil_uuid());
}

BOOST_AUTO_TEST_CASE(db) {
    cyng::store store;
    store.create_table(cyng::meta_store(
        "demo-1",
        {cyng::column("id", cyng::TC_INT64), cyng::column("name", cyng::TC_STRING), cyng::column("age", cyng::TC_TIME_POINT)},
        1));
    store.create_table(cyng::meta_store(
        "demo-2",
        {cyng::column("id", cyng::TC_INT64),
         cyng::column("city", cyng::TC_STRING),
         cyng::column("street", cyng::TC_STRING),
         cyng::column("nr", cyng::TC_UINT16)},
        1));
    store.create_table(cyng::meta_store(
        "demo-3",
        {cyng::column("id", cyng::TC_INT64), cyng::column("product", cyng::TC_STRING), cyng::column("price", cyng::TC_FLOAT)},
        1));

    store.access(
        [](cyng::table const *tbl1, cyng::table *tbl2) {
            // std::cout << "store.lambda: " << tbl1->meta().get_name() << ", " << tbl2->meta().get_name() << std::endl;
        },
        cyng::access::read("demo-1"),
        cyng::access::write("demo-2"));

    store.access(
        [](cyng::table const *tbl2) {
            // std::cout << "store.lambda: " << tbl2->meta().get_name() << std::endl;
        },
        cyng::access::read("demo-2"));

    store.access(
        [](cyng::table const *tbl1, cyng::table *tbl2, cyng::table *tbl3) {
            // std::cout << "store.lambda: "
            // 	<< tbl1->meta().get_name() << ", "
            // 	<< tbl2->meta().get_name() << ", "
            // 	<< tbl3->meta().get_name()
            // 	<< std::endl;
        },
        cyng::access::read("demo-1"),
        cyng::access::write("demo-2"),
        cyng::access::write("demo-3"));
}

class data_sink : public cyng::slot_interface {

  public:
    virtual bool
    forward(cyng::table const *tbl, cyng::key_t const &, cyng::data_t const &, std::uint64_t, boost::uuids::uuid) override {

        // std::cout
        //	<< "insert "
        //	<< tbl->meta().get_name()
        //	<< std::endl;
        return true;
    }

    virtual bool forward(
        cyng::table const *tbl,
        cyng::key_t const &key,
        cyng::attr_t const &attr,
        cyng::data_t const &data,
        std::uint64_t gen,
        boost::uuids::uuid tag) override {
        return true;
    }

    virtual bool
    forward(cyng::table const *tbl, cyng::key_t const &key, cyng::data_t const &data, boost::uuids::uuid tag) override {
        return true;
    }

    virtual bool forward(cyng::table const *, boost::uuids::uuid) override { return true; }

    virtual bool forward(cyng::table const *, bool) override { return true; }
};

BOOST_AUTO_TEST_CASE(slot) {
    cyng::store store;
    store.create_table(cyng::meta_store(
        "demo",
        {cyng::column("id", cyng::TC_INT64), cyng::column("name", cyng::TC_STRING), cyng::column("age", cyng::TC_TIME_POINT)},
        1));

    auto sp = cyng::make_slot(new data_sink());
    store.connect("demo", sp);

    auto const key = cyng::key_generator(12u);
    store.insert(
        "demo",
        key,
        cyng::data_generator("Y", std::chrono::system_clock::now()),
        1u //	gen
        ,
        boost::uuids::nil_uuid());

    store.disconnect("demo", sp);
}

bool test_loop(std::uint64_t ui, std::string s, std::chrono::system_clock::time_point tp) {
    // std::cout << ui << ", " << s << ", " << tp << std::endl;
    BOOST_REQUIRE(ui == 12U || ui == 42);
    return ui == 42U;
}

BOOST_AUTO_TEST_CASE(loop) {
    cyng::meta_store const m(
        "demo",
        {cyng::column("id", cyng::TC_UINT64), cyng::column("name", cyng::TC_STRING), cyng::column("age", cyng::TC_TIME_POINT)},
        1);

    cyng::table tbl(m);

    tbl.insert(
        cyng::key_generator(static_cast<std::uint64_t>(12u)),
        cyng::data_generator("A", std::chrono::system_clock::now()),
        1u //	gen
        ,
        boost::uuids::nil_uuid());

    tbl.insert(
        cyng::key_generator(static_cast<std::uint64_t>(42u)),
        cyng::data_generator("O", std::chrono::system_clock::now()),
        1u //	gen
        ,
        boost::uuids::nil_uuid());

    std::function<bool(std::uint64_t, std::string, std::chrono::system_clock::time_point)> f = test_loop;
    tbl.loop(f);
    // tbl.loop<std::uint64_t, std::string, std::chrono::system_clock::time_point>([](std::uint64_t ui, std::string s,
    // std::chrono::system_clock::time_point tp) {
    // 	//std::cout << ui << ", " << s << ", " << tp << std::endl;
    // 	BOOST_REQUIRE(ui == 12U || ui == 42);
    // 	return true;
    // 	});
}

BOOST_AUTO_TEST_CASE(compute) {
    cyng::meta_store const m(
        "demo",
        {cyng::column("id", cyng::TC_UINT64), cyng::column("name", cyng::TC_STRING), cyng::column("age", cyng::TC_TIME_POINT)},
        1);

    cyng::table tbl(m);

    auto const key = cyng::key_generator(static_cast<std::uint64_t>(12u));
    tbl.insert(
        cyng::key_generator(static_cast<std::uint64_t>(12u)),
        cyng::data_generator("A", std::chrono::system_clock::now()),
        1u //	gen
        ,
        boost::uuids::nil_uuid());

    auto const b = tbl.compute<std::string>(
        key, "name", [](std::string s) -> std::string { return s + s; }, boost::uuids::nil_uuid());

    BOOST_REQUIRE(b);
    auto const rec = tbl.lookup(key);
    auto const s = rec.value("name", "");
    BOOST_REQUIRE_EQUAL(s, "AA");
}
BOOST_AUTO_TEST_SUITE_END()
