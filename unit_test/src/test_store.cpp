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
	docscript::meta_store const m("demo"
		, { 
			docscript::column("id", docscript::TC_INT64), 
			docscript::column("name", docscript::TC_STRING),
			docscript::column("age", docscript::TC_TIME_POINT)
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
	docscript::meta_store const m("demo"
		, {
			docscript::column("id", docscript::TC_INT64),
			docscript::column("name", docscript::TC_STRING),
			docscript::column("age", docscript::TC_TIME_POINT)
		}
	, 1 );

	docscript::table tbl(m);
	auto const key = docscript::key_generator(12u);
	tbl.insert(key
		, docscript::data_generator("A", std::chrono::system_clock::now())
		, 1u	//	gen
		, boost::uuids::nil_uuid());

	//std::cout << tbl.size() << std::endl;
	BOOST_REQUIRE_EQUAL(tbl.size(), 1);
	BOOST_REQUIRE(tbl.exist(key));

	tbl.insert(docscript::key_generator(42u)
		, docscript::data_generator("O", std::chrono::system_clock::now())
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
	docscript::meta_store const m("demo"
		, {
			docscript::column("id", docscript::TC_INT64),
			docscript::column("name", docscript::TC_STRING),
			docscript::column("age", docscript::TC_TIME_POINT)
		}
	, 1);

	auto const key = docscript::key_generator<std::uint64_t>(2u);


	docscript::auto_table tbl(m, key, [](docscript::key_t const& key) {
		BOOST_CHECK_EQUAL(key.size(), 1);
		auto tmp = docscript::value_cast<std::uint64_t>(key.at(0), 0);
		BOOST_REQUIRE_EQUAL(tmp, 2);
		return docscript::key_generator(tmp + 1);
		});

	BOOST_REQUIRE(tbl.is_auto());

	tbl.insert(docscript::data_generator("Q", std::chrono::system_clock::now()), boost::uuids::nil_uuid());

}

BOOST_AUTO_TEST_CASE(db)
{
	docscript::db store;
	store.create_table(docscript::meta_store("demo-1"
		, {
			docscript::column("id", docscript::TC_INT64),
			docscript::column("name", docscript::TC_STRING),
			docscript::column("age", docscript::TC_TIME_POINT)
		}
	, 1));
	store.create_table(docscript::meta_store("demo-2"
		, {
			docscript::column("id", docscript::TC_INT64),
			docscript::column("city", docscript::TC_STRING),
			docscript::column("street", docscript::TC_STRING),
			docscript::column("nr", docscript::TC_UINT16)
		}
	, 1));
	store.create_table(docscript::meta_store("demo-3"
		, {
			docscript::column("id", docscript::TC_INT64),
			docscript::column("product", docscript::TC_STRING),
			docscript::column("price", docscript::TC_FLOAT)
		}
	, 1));

	store.access([](docscript::table const* tbl1, docscript::table* tbl2) {
		// std::cout << "store.lambda: " << tbl1->meta().get_name() << ", " << tbl2->meta().get_name() << std::endl;
		}, docscript::access::read("demo-1"), docscript::access::write("demo-2"));

	store.access([](docscript::table const* tbl2) {
		// std::cout << "store.lambda: " << tbl2->meta().get_name() << std::endl;
		}, docscript::access::read("demo-2"));

	store.access([](docscript::table const* tbl1, docscript::table* tbl2, docscript::table* tbl3) {
		// std::cout << "store.lambda: " 
		// 	<< tbl1->meta().get_name() << ", " 
		// 	<< tbl2->meta().get_name() << ", "
		// 	<< tbl3->meta().get_name()
		// 	<< std::endl;
		}, docscript::access::read("demo-1"), docscript::access::write("demo-2"), docscript::access::write("demo-3"));

}
BOOST_AUTO_TEST_SUITE_END()