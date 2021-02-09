#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/store/meta.h>
#include <cyng/store/auto_table.h>
#include <cyng/store/db.h>

#include <cyng/obj/tag.hpp>
#include <cyng/io/ostream.h>
#include <cyng/obj/value_cast.hpp>

#include <boost/uuid/nil_generator.hpp>

BOOST_AUTO_TEST_SUITE(store_suite)

BOOST_AUTO_TEST_CASE(meta)
{
	cyng::meta_store const m("demo"
		, { 
			cyng::column("id", cyng::TC_INT64), 
			cyng::column("name", cyng::TC_STRING),
			cyng::column("age", cyng::TC_TIME_POINT)
		}
		, 1);
	//std::cerr << m.get_index_by_name("id") << std::endl;
	BOOST_REQUIRE_EQUAL(m.get_index_by_name("id"), 0);
	BOOST_REQUIRE_EQUAL(m.get_index_by_name("name"), 1);
	BOOST_REQUIRE_EQUAL(m.get_index_by_name("age"), 2);
	BOOST_REQUIRE(m.is_pk("id"));
	BOOST_REQUIRE(!m.is_pk("name"));
	BOOST_REQUIRE(!m.is_pk("age"));
}

BOOST_AUTO_TEST_CASE(table)
{
	cyng::meta_store const m("demo"
		, {
			cyng::column("id", cyng::TC_INT64),
			cyng::column("name", cyng::TC_STRING),
			cyng::column("age", cyng::TC_TIME_POINT)
		}
	, 1 );

	cyng::table tbl(m);
	auto const key = cyng::key_generator(12u);
	tbl.insert(key
		, cyng::data_generator("A", std::chrono::system_clock::now())
		, 1u	//	gen
		, boost::uuids::nil_uuid());

	//std::cout << tbl.size() << std::endl;
	BOOST_REQUIRE_EQUAL(tbl.size(), 1);
	BOOST_REQUIRE(tbl.exist(key));

	tbl.insert(cyng::key_generator(42u)
		, cyng::data_generator("O", std::chrono::system_clock::now())
		, 1u	//	gen
		, boost::uuids::nil_uuid());

	BOOST_REQUIRE_EQUAL(tbl.size(), 2);

	auto const rec = tbl.lookup(key);
	auto tpl = rec.to_tuple();
	// std::cout << tpl << std::endl;

	auto rec2 = tbl.find_first("name", std::string("O"));
	// std::cout << rec2.to_tuple() << std::endl;

}

BOOST_AUTO_TEST_CASE(auto_table)
{
	cyng::meta_store const m("demo"
		, {
			cyng::column("id", cyng::TC_INT64),
			cyng::column("name", cyng::TC_STRING),
			cyng::column("age", cyng::TC_TIME_POINT)
		}
	, 1);

	auto const key = cyng::key_generator<std::uint64_t>(2u);


	cyng::auto_table tbl(m, key, [](cyng::key_t const& key) {
		BOOST_CHECK_EQUAL(key.size(), 1);
		auto tmp = cyng::value_cast<std::uint64_t>(key.at(0), 0);
		BOOST_REQUIRE_EQUAL(tmp, 2);
		return cyng::key_generator(tmp + 1);
		});

	BOOST_REQUIRE(tbl.is_auto());

	tbl.insert(cyng::data_generator("Q", std::chrono::system_clock::now()), boost::uuids::nil_uuid());

}

BOOST_AUTO_TEST_CASE(db)
{
	cyng::db store;
	store.create_table(cyng::meta_store("demo-1"
		, {
			cyng::column("id", cyng::TC_INT64),
			cyng::column("name", cyng::TC_STRING),
			cyng::column("age", cyng::TC_TIME_POINT)
		}
	, 1));
	store.create_table(cyng::meta_store("demo-2"
		, {
			cyng::column("id", cyng::TC_INT64),
			cyng::column("city", cyng::TC_STRING),
			cyng::column("street", cyng::TC_STRING),
			cyng::column("nr", cyng::TC_UINT16)
		}
	, 1));
	store.create_table(cyng::meta_store("demo-3"
		, {
			cyng::column("id", cyng::TC_INT64),
			cyng::column("product", cyng::TC_STRING),
			cyng::column("price", cyng::TC_FLOAT)
		}
	, 1));

	store.access([](cyng::table const* tbl1, cyng::table* tbl2) {
		// std::cout << "store.lambda: " << tbl1->meta().get_name() << ", " << tbl2->meta().get_name() << std::endl;
		}, cyng::access::read("demo-1"), cyng::access::write("demo-2"));

	store.access([](cyng::table const* tbl2) {
		// std::cout << "store.lambda: " << tbl2->meta().get_name() << std::endl;
		}, cyng::access::read("demo-2"));

	store.access([](cyng::table const* tbl1, cyng::table* tbl2, cyng::table* tbl3) {
		// std::cout << "store.lambda: " 
		// 	<< tbl1->meta().get_name() << ", " 
		// 	<< tbl2->meta().get_name() << ", "
		// 	<< tbl3->meta().get_name()
		// 	<< std::endl;
		}, cyng::access::read("demo-1"), cyng::access::write("demo-2"), cyng::access::write("demo-3"));

}
BOOST_AUTO_TEST_SUITE_END()