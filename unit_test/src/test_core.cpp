#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE unit_test
#endif

#include <boost/test/unit_test.hpp>
#include <cyng/obj/object.h>
#include <cyng/obj/object_cast.hpp>
#include <cyng/obj/container_factory.hpp>
#include <cyng/obj/buffer_cast.hpp>
#include <cyng/obj/numeric_cast.hpp>
#include <cyng/obj/tuple_cast.hpp>
#include <cyng/obj/function_cast.hpp>
#include <cyng/obj/container_cast.hpp>
#include <cyng.h>

#include <boost/uuid/nil_generator.hpp>

BOOST_AUTO_TEST_SUITE(core_suite)

BOOST_AUTO_TEST_CASE(constructor)
{
    docscript::object obj;
    BOOST_REQUIRE_EQUAL(obj.hash(), cyng_NULL_HASH);
    //std::cout << obj.hash() << std::endl;
    {
        auto const o = obj.clone();
        //std::cout << o.hash() << std::endl;
        BOOST_REQUIRE_EQUAL(o.hash(), cyng_NULL_HASH);
    }
    //std::cout << obj.rtti().extend() << std::endl;
    BOOST_REQUIRE_EQUAL(obj.rtti().extend(), 0);

    //std::cout << obj.rtti().tag() << std::endl;
    BOOST_REQUIRE_EQUAL(obj.rtti().tag(), docscript::TC_NULL);
    BOOST_REQUIRE_EQUAL(obj.rtti().type_name(), "null");

    auto o2 = docscript::make_object(2);
    BOOST_REQUIRE_EQUAL(*docscript::object_cast<int>(o2), 2);

    auto o3 = docscript::make_object("hello");
    auto o4 = docscript::make_object(o3);   //  clone

    auto o5 = docscript::make_object(docscript::make_buffer("buffer"));
    BOOST_REQUIRE_EQUAL(o5.rtti().type_name(), "binary");

    auto o6 = docscript::make_object(new int(42));
    BOOST_REQUIRE_EQUAL(docscript::value_cast<int>(o6, 0), 42);

    auto o7 = docscript::make_object<docscript::buffer_t>(std::string("string"));
    BOOST_REQUIRE_EQUAL(o7.rtti().type_name(), "binary");

    auto i = docscript::to_numeric<int>(docscript::buffer_t{ 1,2,3,4 });
    BOOST_REQUIRE_EQUAL(i, 0x01020304);

    auto const buf = docscript::to_buffer(0x01020304);
    BOOST_REQUIRE_EQUAL(buf.size(), 4);
    BOOST_REQUIRE_EQUAL(buf.at(0), 1u);
    BOOST_REQUIRE_EQUAL(buf.at(1), 2u);
    BOOST_REQUIRE_EQUAL(buf.at(2), 3u);
    BOOST_REQUIRE_EQUAL(buf.at(3), 4u);

    //
    //  produce a vector of objects
    //
    std::vector<std::uint16_t> const v16 = { 2, 0, 2, 1 };
    auto o8 = docscript::make_object<std::vector<std::uint16_t>>(v16);
    BOOST_REQUIRE_EQUAL(o8.rtti().type_name(), "vec");
    auto * p = docscript::object_cast<docscript::vector_t>(o8);
    BOOST_CHECK(p);
    BOOST_REQUIRE_EQUAL(p->size(), 4);

    //
    //  same result
    //
    auto o9 = docscript::make_object(v16);
    BOOST_REQUIRE_EQUAL(o9.rtti().type_name(), "vec");

    //
    //  MAC
    //
    auto o10 = docscript::make_object(docscript::broadcast_address());
    BOOST_REQUIRE_EQUAL(o10.rtti().type_name(), "mac48");

    //std::cout << o10.rtti().type_name();

    //
    //  chrono
    //
    BOOST_REQUIRE_EQUAL(docscript::make_object(std::chrono::nanoseconds(0)).rtti().type_name(), "chrono:ns");
    BOOST_REQUIRE_EQUAL(docscript::make_object(std::chrono::microseconds(0)).rtti().type_name(), "chrono:us");
    BOOST_REQUIRE_EQUAL(docscript::make_object(std::chrono::milliseconds(0)).rtti().type_name(), "chrono:ms");
    BOOST_REQUIRE_EQUAL(docscript::make_object(std::chrono::seconds(0)).rtti().type_name(), "chrono:sec");
    BOOST_REQUIRE_EQUAL(docscript::make_object(std::chrono::minutes(0)).rtti().type_name(), "chrono:min");
    BOOST_REQUIRE_EQUAL(docscript::make_object(std::chrono::hours(0)).rtti().type_name(), "chrono:h");

    //
    //  Boost
    //
   
    BOOST_REQUIRE_EQUAL(docscript::make_object(boost::system::errc::make_error_code(boost::system::errc::success)).rtti().type_name(), "ec");
    BOOST_REQUIRE_EQUAL(docscript::make_object(boost::uuids::nil_uuid()).rtti().type_name(), "uuid");

    //
    //  Boost.Asio
    //
    auto addr = docscript::make_object(boost::asio::ip::make_address("1.1.1.1"));
    BOOST_REQUIRE_EQUAL(addr.rtti().type_name(), "ip:address");
    auto* paddr = docscript::object_cast<boost::asio::ip::address>(addr);
    BOOST_CHECK(paddr);
    BOOST_REQUIRE_EQUAL(paddr->to_string(), "1.1.1.1");

    BOOST_REQUIRE_EQUAL(docscript::make_object(boost::asio::ip::tcp::endpoint()).rtti().type_name(), "ip:tcp:ep");
    BOOST_REQUIRE_EQUAL(docscript::make_object(boost::asio::ip::udp::endpoint()).rtti().type_name(), "ip:udp:ep");
    BOOST_REQUIRE_EQUAL(docscript::make_object(boost::asio::ip::icmp::endpoint()).rtti().type_name(), "ip:icmp:ep");
}

BOOST_AUTO_TEST_CASE(type_name_test)
{
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::null>(), "null");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<bool>(), "bool");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<char>(), "char");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<float>(), "f");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<double>(), "d");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<long double>(), "ld");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::uint8_t>(), "u8");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::uint16_t>(), "u16");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::uint32_t>(), "u32");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::uint64_t>(), "u64");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::int8_t>(), "i8");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::int16_t>(), "i16");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::int32_t>(), "i32");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::int64_t>(), "i64");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::string>(), "s");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::filesystem::path>(), "fs:path");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::chrono::system_clock::time_point>(), "chrono:tp");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::chrono::nanoseconds>(), "chrono:ns");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::chrono::microseconds>(), "chrono:us");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::chrono::milliseconds>(), "chrono:ms");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::chrono::seconds>(), "chrono:sec");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::chrono::minutes>(), "chrono:min");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<std::chrono::hours>(), "chrono:h");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::buffer_t>(), "binary");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::version>(), "v");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::revision>(), "r");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::op>(), "op");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::severity>(), "severity");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::mac48>(), "mac48");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::mac64>(), "mac64");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::obis>(), "obis");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::crypto::digest_md5>(), "crypto:MD5");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::crypto::digest_sha1>(), "crypto:SHA1");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::crypto::digest_sha256>(), "crypto:SHA256");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::crypto::digest_sha512>(), "crypto:SHA512");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::tuple_t>(), "tpl");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::vector_t>(), "vec");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::deque_t>(), "deque");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::attr_map_t>(), "amap");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::param_map_t>(), "pmap");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::param_t>(), "param");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<boost::system::error_code>(), "ec");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<boost::uuids::uuid>(), "uuid");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<boost::asio::ip::address>(), "ip:address");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<boost::asio::ip::tcp::endpoint>(), "ip:tcp:ep");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<boost::asio::ip::udp::endpoint>(), "ip:udp:ep");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<boost::asio::ip::icmp::endpoint>(), "ip:icmp:ep");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::eod>(), "eod");
    BOOST_REQUIRE_EQUAL(docscript::intrinsic_name<docscript::object>(), "obj");
}

struct dummy
{
    inline void foo() {

    }
    inline int bar(int a, std::string b, float c) {
        std::cout << a + 1 << ", " << b << ": " << c << std::endl;
        return 1 + a;
    }
};


BOOST_AUTO_TEST_CASE(cast)
{
    dummy d;

    auto o6 = docscript::make_object(new int(42));
    auto* ptr = docscript::object_cast<int>(o6);
    BOOST_REQUIRE(ptr != nullptr);
    BOOST_REQUIRE_EQUAL(*ptr, 42);

    auto n16 = docscript::numeric_cast<std::uint16_t>(o6, 16);
    BOOST_REQUIRE_EQUAL(n16, 42);

    BOOST_REQUIRE(o6); 
    auto ptr1 = docscript::object_release<std::uint16_t>(o6);
    BOOST_REQUIRE(!o6); //  empty
    BOOST_REQUIRE_EQUAL(*ptr1.first, 42);
    delete ptr1.second;    //  prevent memory leak

    auto tpl = docscript::make_tuple(1, "hello", 2.3f);
    BOOST_REQUIRE_EQUAL(tpl.size(), 3); 

    using ft = std::function<int(int, std::string, float)>;
    ft f = [](int a, std::string b, float c)->int {
        // std::cout << a << b << c << std::endl;
        return a + 1;
    };
    auto const s1 = docscript::function_cast<ft>(tpl);
    BOOST_REQUIRE_EQUAL(std::tuple_size_v<decltype(s1)>, 3);
    BOOST_REQUIRE_EQUAL(std::get<0>(s1), 1);
    BOOST_REQUIRE_EQUAL(std::get<1>(s1), "hello");
    BOOST_REQUIRE_CLOSE_FRACTION(std::get<2>(s1), 2.3f, 0.00001);

    auto const s2 = docscript::function_cast<decltype(f)>(tpl);
    BOOST_REQUIRE_EQUAL(std::tuple_size_v<decltype(s2)>, 3);
    BOOST_REQUIRE_EQUAL(std::get<0>(s2), 1);
    BOOST_REQUIRE_EQUAL(std::get<1>(s2), "hello");
    BOOST_REQUIRE_CLOSE_FRACTION(std::get<2>(s2), 2.3f, 0.00001);

    auto const s3 = docscript::function_call<ft>(f, tpl);
    BOOST_REQUIRE_EQUAL(s3, 2);

    //
    //  container cast
    //
    auto obj = docscript::make_object(docscript::make_tuple(1, 2, 3.4));
    BOOST_REQUIRE(obj);
    auto tpl2 = docscript::container_cast<docscript::tuple_t>(obj);
    BOOST_REQUIRE(!obj);
    BOOST_REQUIRE_EQUAL(tpl2.size(), 3);

    auto const o7 = docscript::make_object(docscript::make_tuple(1, 2, 3.4));
    BOOST_REQUIRE(o7);
    auto tpl7 = docscript::container_cast<docscript::tuple_t>(o7);
    BOOST_REQUIRE(o7);
    BOOST_REQUIRE_EQUAL(tpl7.size(), 3);

    // auto r2 = container_transform<docscript::vector_t>(docscript::make_tuple(1, 2, 3.4));
    // BOOST_REQUIRE_EQUAL(r2.size(), 3);
}

BOOST_AUTO_TEST_CASE(obis)
{
    docscript::obis o(1, 2, 3, 4, 5, 6);
    //std::cout << docscript::to_str(o) << std::endl;
    BOOST_REQUIRE_EQUAL(docscript::to_str(o), "010203040506");
    //std::cout << std::hex << o.to_uint64() << std::endl;
    BOOST_REQUIRE_EQUAL(o.to_uint64(), 0x010203040506);
}

BOOST_AUTO_TEST_SUITE_END()