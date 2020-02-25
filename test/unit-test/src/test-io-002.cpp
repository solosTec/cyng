
/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Sylko Olzscher
 *
 */
#include "test-io-002.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/factory.h>
#include <cyng/type.h>
#include <cyng/crypto/hash/md5.h>
#include <cyng/crypto/hash/sha1.h>
#include <cyng/crypto/hash/sha256.h>
#include <cyng/crypto/hash/sha512.h>
#include <cyng/io/serializer.h>
#include <cyng/io/parser/parser.h>

#include <cyng/io/parser/deserializer.h>

#include <boost/uuid/string_generator.hpp>
#include <boost/math/constants/constants.hpp>

namespace cyng
{
//
//	define a custom object
//
class custom
{
public:
    custom()
    {}
};

namespace traits
{
template <>
struct type_tag<custom> {
    using type = custom;
    using tag = std::integral_constant<std::size_t,
#if defined(__CPP_SUPPORT_N2347)
          static_cast<std::size_t> ( traits::predef_type_code::PREDEF_CUSTOM )
#else
          PREDEF_CUSTOM
#endif
          >;

#if defined(__CPP_SUPPORT_N2235)
    constexpr static char name[] = "custom";
#else
    const static char name[];
#endif
};

#if !defined(__CPP_SUPPORT_N2235)
const char type_tag<custom>::name[] = "custom";
#endif

template <>
struct reverse_type <
#if defined(__CPP_SUPPORT_N2347)
static_cast<std::size_t> ( traits::predef_type_code::PREDEF_CUSTOM )
#else
    PREDEF_CUSTOM
#endif
> {
    using type = custom;
};
}
}

#include <functional>
#include <boost/functional/hash.hpp>

namespace std
{

template<>
struct hash<cyng::custom> {
    inline size_t operator() ( cyng::custom const& s ) const noexcept
    {
        return 0;
    }
};
template<>
struct equal_to<cyng::custom> {
    using result_type = bool;
    using first_argument_type = cyng::custom;
    using second_argument_type = cyng::custom;

    inline bool operator() ( cyng::custom const& s1, cyng::custom const& s2 ) const noexcept
    {
        return false;
    }
};
template<>
struct less<cyng::custom> {
    using result_type = bool;
    using first_argument_type = cyng::custom;
    using second_argument_type = cyng::custom;

    inline bool operator() ( cyng::custom const& s1, cyng::custom const& s2 ) const noexcept
    {
        return false;
    }
};
}

namespace cyng
{

namespace xyz
{
/**
	* helper class to justify and calculate the size of
	* the length field.
	*/
class length_field
{
public:
    length_field();

    /**
    	* Add a new char to the field.
    	*
    	* @return true if length field is complete
    	*/
    bool put ( char c );

    /**
    	* reset internal values
    	*/
    void reset();

    /**
    	* @return the calculated length.
    	*/
    std::uint64_t length() const;

    /**
    	* @return true if length field is complete
    	*/
    bool is_complete() const;

    /**
    	* @return true if the calculated length is null
    	*/
    bool is_null() const;

    /*
    	* set value of length field
    	*/
    void set ( std::size_t );

private:
    /**
    	* @return size of length field in bytes.
    	*/
    std::size_t size_of_length_field() const;

private:
    constexpr static std::size_t size_{ sizeof ( std::uint64_t ) + sizeof ( char ) };
    union
#ifdef BOOST_COMP_GNUC
        __attribute__ ( ( __packed__ ) )
#endif
    {
        unsigned char source_[size_];
        struct {
            char dummy_;
            std::uint16_t l_;
        } l16_;
        struct {
            char dummy_;
            std::uint64_t l_;
        } l64_;
    } u_;
    std::size_t pos_;
};

/**
	* Data type is encoded in 4 bytes
	*/
class type_field
{
public:
    type_field();

    /**
    	* Add a new char to the field.
    	*
    	* @return true if length field is complete
    	*/
    bool put ( char c );

    /**
    	* reset internal values
    	*/
    void reset();

    /**
    	* @return the received data type code.
    	*/
    std::size_t type() const;

    /**
    	* @return true if type field is complete
    	*/
    bool is_complete() const;

private:
    constexpr static std::size_t size_{ sizeof ( std::uint32_t ) };
    union
#ifdef BOOST_COMP_GNUC
        __attribute__ ( ( __packed__ ) )
#endif
    {
        unsigned char source_[size_];
        std::uint32_t l_;
    } u_;
    std::size_t pos_;
};
}	//	xyz

class dummy
{
    enum state {
        NP_ERROR,

        //	t-l-v
        NP_TYPE,
        NP_LENGTH,
        NP_VALUE,
        //NP_EMPTY,

    }	state_;
public:
    using parser_callback = std::function<void ( vector_t&& ) >;

public:
    dummy ( parser_callback cb )
        : state_ ( NP_TYPE )
        , stream_buffer_()
        , input_ ( &stream_buffer_ )
        , type_()
        , length_()
        , cb_ ( cb )
        , code_()
    {
        std::cout << "dummy:dummy()" << std::endl;
        BOOST_ASSERT_MSG ( cb_, "no callback specified" );
    }

    virtual ~dummy()
    {}

    template < typename I >
    std::size_t read ( I start, I end )
    {
        //	doesn't work for plain pointers
        //static_assert(std::is_same<typename I::value_type, char>::value, "wrong iterator type");
        std::for_each ( start, end, [this] ( char c ) {
            this->put ( c );
        } );

        return post_processing();
    }

private:
    void put ( char c )
    {
        switch ( state_ ) {
        case NP_ERROR:
            //	ToDo: error recovery
            break;
        case NP_TYPE:
            state_ = parse_type ( c );
            break;
        case NP_LENGTH:
            state_ = parse_length ( c );
            if ( state_ == NP_TYPE ) {
                BOOST_ASSERT ( type_.is_complete() );
                BOOST_ASSERT ( length_.is_null() );
                //	no data required to create object
                switch ( type_.type() ) {
                case TC_NULL:
                    next ( make_object() );
                    break;
                case TC_EOD:
                    next ( make_object ( eod() ) );
                    break;
                case TC_STRING:
                    next ( make_object ( std::string() ) );
                    break;
                case TC_BUFFER:
                    next ( make_object ( buffer_t() ) );
                    break;

                case TC_TUPLE:
                    next ( make_tuple() );
                    break;
                case TC_VECTOR:
                    next ( make_vector() );
                    break;
                case TC_SET:
                    next ( make_set() );
                    break;
                case TC_ATTR_MAP:
                    next ( attr_map_factory() () );
                    break;
                case TC_PARAM_MAP:
                    next ( param_map_factory() () );
                    break;

                case TC_FS_PATH:
                    next ( make_object ( boost::filesystem::path() ) );
                    break;
                case TC_IP_TCP_ENDPOINT:
                    next ( make_object ( boost::asio::ip::tcp::endpoint() ) );
                    break;
                case TC_IP_UDP_ENDPOINT:
                    next ( make_object ( boost::asio::ip::udp::endpoint() ) );
                    break;
                case TC_IP_ICMP_ENDPOINT:
                    next ( make_object ( boost::asio::ip::icmp::endpoint() ) );
                    break;
                case TC_IP_ADDRESS:
                    next ( make_object ( boost::asio::ip::address() ) );
                    break;

                default:
                    BOOST_ASSERT_MSG ( false, "type requires data" );
                    break;
                }
                type_.reset();
                length_.reset();
            }
            break;
        case NP_VALUE:
			state_ = parse_value(c);
            break;

        default:
            break;
        }

    }
    std::size_t post_processing()
    {
        return post_processing ( stream_buffer_.size() );
    }
    std::size_t post_processing ( std::size_t count )
    {
        //	trigger callback
        const std::size_t ops = code_.size();
        if ( cb_ )	cb_ ( std::move ( code_ ) );
        return ops;
    }

    state parse_type ( char c )
    {
        type_.put ( c );
        if ( !type_.is_complete() ) {
            return state_;
        }
        return NP_LENGTH;
    }

    state parse_length ( char c )
    {
        return ( length_.put ( c ) )
               ? ( length_.is_null() ? NP_TYPE : NP_VALUE )
               : NP_LENGTH
               ;
    }

    void next ( object&& obj )
    {
        code_.push_back ( std::move ( obj ) );
    }

    state parse_value(char c)
	{
		//BOOST_ASSERT_MSG(length_.target_ != 0, "input get lost");
		if (stream_buffer_.size() < length_.length())
		{
			input_.put(c);
		}
		
		if (stream_buffer_.size() == length_.length())
		{
			//	buffer complete - synthesize  object.
			if (built_in_tag(type_.type()))
			{
				next(io::deserialize(input_, type_.type(), length_.length()));
			}
			else
			{
				next(create_custom_object(input_, type_.type(), length_.length()));
			}
			
			//
			//	reset type  and length field
			//
			type_.reset();
			length_.reset();
			
			//
			//	next object
			//
			return NP_TYPE;
		}
		
		return state_;
	}
	
	object create_custom_object(std::istream& is, std::size_t tag, std::uint64_t size)
	{
		//
		//	see binary hpp
		//	serializer_custom<SERIALIZE_BINARY>
		//
		
		//BOOST_ASSERT_MSG(false, "missing overwrite for create_custom_object()");
		std::string v;
		std::istreambuf_iterator<char> eos;
		std::string s(std::istreambuf_iterator<char>(is), eos);
		
		return make_object(s);
	}
	
private:
    boost::asio::streambuf	stream_buffer_;
    std::iostream			input_;

    xyz::type_field	type_;
    xyz::length_field	length_;

    parser_callback	cb_;
    vector_t	code_;
};

void write_test_data(std::ostream& os)
{
	io::serialize_binary ( os, make_object() );	//	test NULL value
	
	io::serialize_binary ( os, make_object ( true ) );
	io::serialize_binary ( os, make_object ( static_cast<char> ( 33 ) ) );	//	ASCII 33 == !, serialized as string
	io::serialize_binary ( os, make_object ( boost::math::constants::e<float>() ) );	//	2.71828
	io::serialize_binary ( os, make_object ( boost::math::constants::pi<double>() ) );	//	3.14159
	//	different sizes on different compilers
	//io::serialize_binary(f, make_object(boost::math::constants::phi<long double>()));	//	1.61803
	
	io::serialize_binary ( os, make_object ( static_cast<std::uint8_t> ( 34 ) ) );
	io::serialize_binary ( os, make_object ( static_cast<std::uint16_t> ( 3 ) ) );
	io::serialize_binary ( os, make_object ( static_cast<std::uint32_t> ( 4 ) ) );
	io::serialize_binary ( os, make_object ( static_cast<std::uint64_t> ( 5 ) ) );
	
	io::serialize_binary ( os, make_object ( static_cast<std::int8_t> ( 35 ) ) );
	io::serialize_binary ( os, make_object ( static_cast<std::int16_t> ( 7 ) ) );
	io::serialize_binary ( os, make_object ( static_cast<std::int32_t> ( 8 ) ) );
	io::serialize_binary ( os, make_object ( static_cast<std::int64_t> ( 9 ) ) );
	
	io::serialize_binary ( os, make_object ( "" ) );
	io::serialize_binary ( os, make_object ( "hello, world!" ) );
	
	io::serialize_binary ( os, make_object ( chrono::init_tp ( 2018, 4, 20, 6, 5, 32 ) ) );
	io::serialize_binary ( os, make_object ( std::chrono::nanoseconds ( 10 ) ) );
	io::serialize_binary ( os, make_object ( std::chrono::microseconds ( 11 ) ) );
	io::serialize_binary ( os, make_object ( std::chrono::milliseconds ( 12 ) ) );
	io::serialize_binary ( os, make_object ( std::chrono::seconds ( 13 ) ) );
	io::serialize_binary ( os, make_object ( std::chrono::minutes ( 14 ) ) );
	io::serialize_binary ( os, make_object ( std::chrono::hours ( 15 ) ) );
	io::serialize_binary ( os, make_object ( cyng::chrono::to_dbl_time_point ( chrono::init_tp ( 2018, 4, 20, 6, 5, 33 ) ) ) );
	//io::serialize_binary(f, make_object(cyng::chrono::convert(std::chrono::system_clock::now())));
	
	io::serialize_binary ( os, make_object ( cyng::version ( 1, 5 ) ) );
	io::serialize_binary ( os, make_object ( cyng::revision ( cyng::version ( 1, 6 ), cyng::version ( 7, 8 ) ) ) );
	
	//code,
	io::serialize_binary ( os, make_object ( cyng::code::ESBA ) );
	//label,
	//logging::severity,
	io::serialize_binary ( os, make_object ( cyng::logging::severity::LEVEL_WARNING ) );
	//buffer_t,
	io::serialize_binary ( os, make_object ( cyng::buffer_t{16, 17, 18} ) );
	//mac48,
	io::serialize_binary ( os, make_object ( cyng::mac48 ( 0x00, 0xFF, 0xB0, 0x4B, 0xBE, 0xAA ) ) );
	//mac64,
	io::serialize_binary ( os, make_object ( cyng::mac64 ( 0x00, 0xFF, 0xB0, 0x4B, 0xBE, 0xAA, 0xBB, 0xCC ) ) );
	//color_8,
	//color_16,
	
	//crypto::digest_md5,
	io::serialize_binary ( os, make_object ( crypto::digest_md5 ( md5_hash ( "hash me" ) ) ) );
	//crypto::digest_sha1,
	io::serialize_binary ( os, make_object ( crypto::digest_sha1 ( sha1_hash ( "hash me" ) ) ) );
	//crypto::digest_sha256,
	io::serialize_binary ( os, make_object ( crypto::digest_sha256 ( sha256_hash ( "hash me" ) ) ) );
	//crypto::digest_sha512,
	io::serialize_binary ( os, make_object ( crypto::digest_sha512 ( sha512_hash ( "hash me" ) ) ) );
	
	//object, 		//	embedded object - don't do this
	
	//tuple_t,
	//vector_t,
	io::serialize_binary ( os, make_object ( vector_factory ( { 1, 2, 3 } ) ) );
	io::serialize_binary ( os, make_vector() );	//	empty vector
	//set_t,
	
	//attr_map_t,		//	std::map<std::size_t, object>;
	//attr_t,			//	std::pair<std::size_t, object>;
	//param_map_t,	//	std::map<std::string, object>;
	//param_t,		//	std::pair<std::string, object>;
	
	//lockable,
	
	//boost::system::error_code,
	//boost::uuids::uuid,
	io::serialize_binary ( os, make_object ( boost::uuids::string_generator() ( "2f28413a-d69f-4fc6-b39b-14ff401b15d2" ) ) );
	//boost::filesystem::path,
	io::serialize_binary ( os, make_object ( boost::filesystem::path ( "demo.txt" ) ) );
	//boost::asio::ip::tcp::endpoint,
	io::serialize_binary ( os, make_object ( boost::asio::ip::tcp::endpoint ( boost::asio::ip::address::from_string ( "127.0.0.1" ), 20015 ) ) );
	//boost::asio::ip::udp::endpoint,
	io::serialize_binary ( os, make_object ( boost::asio::ip::udp::endpoint ( boost::asio::ip::address::from_string ( "127.0.0.1" ), 20016 ) ) );
	//boost::asio::ip::icmp::endpoint,
	io::serialize_binary ( os, make_object ( boost::asio::ip::icmp::endpoint ( boost::asio::ip::address::from_string ( "127.0.0.1" ), 20017 ) ) );
	//boost::asio::ip::address,
	io::serialize_binary ( os, make_object ( boost::asio::ip::address::from_string ( "172.16.254.1" ) ) );
	
	//io::serialize_binary(f, make_object(0xAA55AA55)); // not portable!
	
	//
	//	dealing with large values
	//
#ifdef _TEST_LARGE_TYPES
	cyng::random_string_factory rsf ( "abcdefghijklmnopqrstuvwxyz" );
	io::serialize_binary ( os, rsf ( 1000 ) );
#endif
	
	//
	//	custom objects
	//
#ifdef _TEST_CUSTOM_TYPES
	custom c;
	io::serialize_binary ( os, make_object ( c ) );
#endif
	
}

void read_test_data ( vector_t&& prg )
{
	//	empty callback
	//             std::cout << prg.size() << std::endl;
	BOOST_CHECK_EQUAL ( prg.size(), 47 );
	std::size_t index{ 0 };
	for ( auto obj : prg ) {
		//                 std::cout << "#" << index << ": ";
		//                 io::serialize_plain ( std::cout, obj );
		//                 std::cout << " ";
		
		auto const str = io::to_str ( obj );
		
		switch ( index ) {
			case 0:
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_NULL );
				BOOST_CHECK_EQUAL ( str, "null" );
				break;
			case 1:
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_BOOL );
				BOOST_CHECK_EQUAL ( str, "true" );
				break;
			case 2:
				//	get string (15)! - serialized as string
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_STRING );
				BOOST_CHECK_EQUAL ( str, "!" );
				break;
			case 3:
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_FLOAT );
				BOOST_CHECK_EQUAL ( str, "2.71828" );
				break;
			case 4:
				//d
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_DOUBLE );
				BOOST_CHECK_EQUAL ( str, "3.14159" );
				break;
			case 5:
				//u8
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_UINT8 );
				BOOST_CHECK_EQUAL ( str, "22" );
				break;
			case 6:
				//u16
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_UINT16 );
				BOOST_CHECK_EQUAL ( str, "0003" );
				break;
			case 7:
				//u32
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_UINT32 );
				BOOST_CHECK_EQUAL ( str, "00000004" );
				break;
			case 8:
				//u64
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_UINT64 );
				BOOST_CHECK_EQUAL ( str, "5" );
				break;
			case 9:
				//i8
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_INT8 );
				BOOST_CHECK_EQUAL ( str, "35" );
				break;
			case 10:
				//i16
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_INT16 );
				BOOST_CHECK_EQUAL ( str, "7" );
				break;
			case 11:
				//i32
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_INT32 );
				BOOST_CHECK_EQUAL ( str, "8" );
				break;
			case 12:
				//i64
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_INT64 );
				BOOST_CHECK_EQUAL ( str, "9" );
				break;
			case 13:
				//s
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_STRING );
				break;
			case 14:
				//s
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_STRING );
				BOOST_CHECK_EQUAL ( str, "hello, world!" );
				break;
			case 15:
				//	chrono:tp
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_TIME_POINT );
				BOOST_CHECK_EQUAL ( str, "2018-04-20 06:05:32.00000000" );
				break;
			case 16:
				//chrono:ns
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_NANO_SECOND );
				BOOST_CHECK_EQUAL ( str, "00:00:0.000000" );
				break;
			case 17:
				//chrono:us
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_MICRO_SECOND );
				BOOST_CHECK_EQUAL ( str, "00:00:0.000011" );
				break;
			case 18:
				//chrono:ms
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_MILLI_SECOND );
				BOOST_CHECK_EQUAL ( str, "00:00:0.012000" );
				break;
			case 19:
				//chrono:sec
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_SECOND );
				BOOST_CHECK_EQUAL ( str, "00:00:13.000000" );
				break;
			case 20:
				//chrono:min
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_MINUTE );
				BOOST_CHECK_EQUAL ( str, "00:14:0.000000" );
				break;
			case 21:
				//chrono:h
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_HOUR );
				BOOST_CHECK_EQUAL ( str, "15:00:0.000000" );
				break;
			case 22:
				//chrono:dtp
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_DBL_TP );
				BOOST_CHECK_EQUAL ( str, "2018-04-20 06:05:33.00000000" );
				break;
			case 23:
				//v
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_VERSION );
				BOOST_CHECK_EQUAL ( str, "1.5" );
				break;
			case 24:
				//rev
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_REVISION );
				BOOST_CHECK_EQUAL ( str, "1.6.7.8" );
				break;
			case 25:
				//op
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_CODE );
				BOOST_CHECK_EQUAL ( str, "op:ESBA" );
				break;
			case 26:
				//log:severity
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_SEVERITY );
				BOOST_CHECK_EQUAL ( str, "WARN " );
				break;
			case 27:
				//binary
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_BUFFER );
				BOOST_CHECK_EQUAL ( str, "101112" );
				break;
			case 28:
				//mac48
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_MAC48 );
				BOOST_CHECK_EQUAL ( str, "00:ff:b0:4b:be:aa" );
				break;
			case 29:
				//mac64
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_MAC64 );
				BOOST_CHECK_EQUAL ( str, "ff00:4bb0:aabe:ccbb" );
				break;
			case 30:
				//crypto:MD5
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_DIGEST_MD5 );
				BOOST_CHECK_EQUAL ( str, "17b31dce96b9d6c6d0a6ba95f47796fb" );
				break;
			case 31:
				//crypto:SHA1
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_DIGEST_SHA1 );
				BOOST_CHECK_EQUAL ( str, "43f932e4f7c6ecd136a695b7008694bb69d517bd" );
				break;
			case 32:
				//crypto:SHA256
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_DIGEST_SHA256 );
				BOOST_CHECK_EQUAL ( str, "eb201af5aaf0d60629d3d2a61e466cfc0fedb517add831ecac5235e1daa963d6" );
				break;
			case 33:
				//crypto:SHA512
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_DIGEST_SHA512 );
				BOOST_CHECK_EQUAL ( str, "8529afcbc87cc6ea6eac37d12b60d9a87095170811b6b61036fda72a5e5446041b29ca48fc20aa3a493f62fd10b55e6ccaca4ce18e7bd7e285ec30929783bf59" );
				break;
			case 34:
				//i32
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_INT32 );
				BOOST_CHECK_EQUAL ( str, "1" );
				break;
			case 35:
				//i32
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_INT32 );
				BOOST_CHECK_EQUAL ( str, "2" );
				break;
			case 36:
				//i32
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_INT32 );
				BOOST_CHECK_EQUAL ( str, "3" );
				break;
			case 37:
				//u64
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_UINT64 );
				BOOST_CHECK_EQUAL ( str, "3" );
				break;
			case 38:
				//op
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_CODE );
				BOOST_CHECK_EQUAL ( str, "op:VEC" );
				break;
			case 39:
				//u64
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_UINT64 );
				BOOST_CHECK_EQUAL ( str, "0" );
				break;
			case 40:
				//op
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_CODE );
				BOOST_CHECK_EQUAL ( str, "op:VEC" );
				break;
			case 41:
				//uuid
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_UUID );
				BOOST_CHECK_EQUAL ( str, "2f28413a-d69f-4fc6-b39b-14ff401b15d2" );
				break;
			case 42:
				//fs:path
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_FS_PATH );
				BOOST_CHECK_EQUAL ( str, "\"demo.txt\"" );
				break;
			case 43:
				//ip:tcp:endpoint
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_IP_TCP_ENDPOINT );
				BOOST_CHECK_EQUAL ( str, "127.0.0.1:20015" );
				break;
			case 44:
				//ip:udp:endpoint
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_IP_UDP_ENDPOINT );
				BOOST_CHECK_EQUAL ( str, "127.0.0.1:20016" );
				break;
			case 45:
				//ip:icmp:endpoint
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_IP_ICMP_ENDPOINT );
				BOOST_CHECK_EQUAL ( str, "127.0.0.1:20017" );
				break;
			case 46:
				//ip:address
				BOOST_CHECK_EQUAL ( obj.get_class().tag(), TC_IP_ADDRESS );
				BOOST_CHECK_EQUAL ( str, "172.16.254.1" );
				break;
				
			default:
				std::cout
				<< index
				<< " "
				<< obj.get_class().type_name()
				<< " "
				<< str
				<< std::endl;
				break;
		}
		++index;
	}
}

bool test_io_002()
{
    const auto tmp = boost::filesystem::temp_directory_path() / "test_io_002.bin";
    const std::string file_name ( tmp.string() );
    {
//         std::cout << file_name << " is open to write" << std::endl;
        std::ofstream f ( file_name, std::ios::binary | std::ios::trunc );
        //BOOST_ASSERT_MSG(f.is_open(), "bad state");
        BOOST_CHECK ( f.is_open() );

		write_test_data(f);
    }
    {
        std::ifstream f ( file_name, std::ios::binary );
        BOOST_CHECK ( f.is_open() );
//         std::cout << file_name << " is open to read (1)" << std::endl;
        //	otherwise all whitespaces get lost
// 			f >> std::noskipws;	//	same as  f.unsetf(std::ios::skipws);
        f.unsetf ( std::ios::skipws );
//         std::cout << file_name << " is open to read (2)" << std::endl;

        static dummy p ( [] ( vector_t&& prg ) {
            std::cout << prg.size() << std::endl;
            std::size_t index{ 0 };
            for ( auto obj : prg ) {
                std::cout << "#" << index << ": ";
                io::serialize_plain ( std::cout, obj );
                std::cout << " ";

                auto const str = io::to_str ( obj );

                ++index;
            }
        } );
		
		static parser np(std::bind(read_test_data, std::placeholders::_1));


//         std::streampos file_size;
//         f.seekg ( 0, std::ios::end );
//         file_size = f.tellg();
//         f.seekg ( 0, std::ios::beg );
// 
//         std::vector<char> vec;
//         vec.reserve ( file_size );
//         vec.insert ( vec.begin(),
//                      std::istream_iterator<char> ( f ),
//                      std::istream_iterator<char>() );

//         std::cout << "start parser " << file_size << "/" << vec.size() << " byte(s)" << std::endl;

//         p.read ( vec.begin(), vec.end() );
// 			p.read(std::istream_iterator<char>(f), std::istream_iterator<char>());
		np.read(std::istream_iterator<char>(f), std::istream_iterator<char>());
        std::cout << std::endl;
    }
    return true;
}

namespace xyz
{

//
//	implementation of length field
//

length_field::length_field()
    : u_()
    , pos_ ( 0 )
{
    reset();
}

std::size_t length_field::size_of_length_field() const
{
    BOOST_ASSERT_MSG ( pos_ != 0, "no length field available" );

    if ( u_.source_[0] == 126 ) {
        //	16 bit length
        //	the following 2 bytes interpreted as a 16
        //	bit unsigned integer are the payload length
        return 3u;
    } else if ( u_.source_[0] == 127 ) {
        //	64 bit length
        //	the following 8 bytes interpreted as a 64 - bit unsigned integer(the
        //	most significant bit MUST be 0) are the payload length
        return 9u;
    }
    //	complete (1 byte)
    //	length is less than 126 bytes
    return 1u;
}

bool length_field::put ( char c )
{
    BOOST_ASSERT ( pos_ != size_ );
    u_.source_[pos_++] = c;
#ifdef DEBUG_CYNG_IO
    if ( is_complete() ) {
        std::cout
                << "length: "
                << length()
                << std::endl
                ;
        return true;
    }
    return false;
#else
    return is_complete();
#endif
}

void length_field::reset()
{
    std::fill ( std::begin ( u_.source_ ), std::end ( u_.source_ ), '\0' );
    pos_ = 0;
}

std::uint64_t length_field::length() const
{
    if ( u_.source_[0] == 126 ) {
        //	16 bit length
        return u_.l16_.l_;
    } else if ( u_.source_[0] == 127 ) {
        //	64 bit length
        return u_.l64_.l_;
    }
    return u_.source_[0];
}

bool length_field::is_complete() const
{
    return ! ( pos_ < size_of_length_field() );
}

bool length_field::is_null() const
{
    return u_.source_[0] == 0;
}

void length_field::set ( std::size_t size )
{
    if ( size < 126 ) {
        u_.source_[0] = static_cast<unsigned char> ( size & 0xff );
        pos_ = 1;
    } else if ( size < 0xffff ) {
        //	little hackish
        u_.source_[0] = 126;
        pos_ = 5;
        u_.l16_.l_ = boost::numeric_cast<std::uint16_t> ( size );
    } else {
        //	little hackish
        u_.source_[0] = 127;
        pos_ = 9;
        u_.l64_.l_ = size;
    }
}

//
//	implementation of type field
//

type_field::type_field()
    : u_()
    , pos_ ( 0 )
{
    reset();
}

bool type_field::put ( char c )
{
    if ( pos_ == size_ ) {
        return true;
    }
    u_.source_[pos_++] = c;
    return is_complete();
}

void type_field::reset()
{
    std::fill ( std::begin ( u_.source_ ), std::end ( u_.source_ ), '\0' );
    pos_ = 0;
}

bool type_field::is_complete() const
{
    return ! ( pos_ < size_ );
}

std::size_t type_field::type() const
{
    //	32 bit length
    return u_.l_;
}
}	//	xyz

}
