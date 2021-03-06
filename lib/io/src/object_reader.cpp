/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/io/object_reader.hpp>

namespace cyng 
{
	namespace io 
	{
		
		std::chrono::system_clock::time_point reader_policy<std::chrono::system_clock::time_point>::extract(std::istream& is)
		{
			chrono::dbl_time_point dtp;
			dtp.first = read_binary<std::time_t>(is);
			dtp.second = read_binary<double>(is);
			return cyng::chrono::to_time_point(dtp);
		}
		
		std::string reader_policy<std::string>::extract(std::istream& is)
		{
			std::string v;
// 			is >> v;
// 			return v;
			std::istreambuf_iterator<char> eos;
			return std::string(std::istreambuf_iterator<char>(is), eos);
			
		}
		
		boost::uuids::uuid reader_policy<boost::uuids::uuid>::extract(std::istream& is)
		{
			//	uint8_t data[16];
			using subtype = decltype(boost::uuids::uuid::data);
			subtype a = {0};
			read_binary(is, a);
			boost::uuids::uuid tag;
			std::copy(std::begin(a), std::end(a), std::begin(tag.data));
			return tag;
		}
		
		boost::filesystem::path reader_policy<boost::filesystem::path>::extract(std::istream& is)
		{
			std::istreambuf_iterator<char> eos;
			return boost::filesystem::path(std::istreambuf_iterator<char>(is), eos);
		}

		chrono::dbl_time_point reader_policy<chrono::dbl_time_point>::extract(std::istream& is)
		{
			//	std::pair<std::time_t, double>
			chrono::dbl_time_point r;
			r.first = read_binary<std::time_t>(is);
			r.second = read_binary<double>(is);
			return r;
		}

		version reader_policy<version>::extract(std::istream& is)
		{
			return version(read_binary<version::full_type>(is));
		}

		revision reader_policy<revision>::extract(std::istream& is)
		{
			return revision(read_binary<revision::full_type>(is));
		}

		code reader_policy<code>::extract(std::istream& is)
		{
			return code(read_binary<code>(is));
		}

		buffer_t reader_policy<buffer_t>::extract(std::istream& is)
		{
			std::istreambuf_iterator<buffer_t::value_type> eos;
			return buffer_t(std::istreambuf_iterator<buffer_t::value_type>(is), eos);
		}

		mac48  reader_policy<mac48>::extract(std::istream& is)
		{
			mac48::address_type a = { 0 };
			read_binary(is, a);
			return mac48(a);
		}

		mac64 reader_policy<mac64>::extract(std::istream& is)
		{
			mac64::address_type a = { 0 };
			read_binary(is, a);
			return mac64(a);
		}

		crypto::digest_md5 reader_policy<crypto::digest_md5>::extract(std::istream& is)
		{
			crypto::digest_md5::value_type a{ 0 };
			read_binary(is, a);
			return crypto::digest_md5(a);
		}

		crypto::digest_sha1 reader_policy<crypto::digest_sha1>::extract(std::istream& is)
		{
			crypto::digest_sha1::value_type a{ 0 };
			read_binary(is, a);
			return crypto::digest_sha1(a);
		}

		crypto::digest_sha256 reader_policy<crypto::digest_sha256>::extract(std::istream& is)
		{
			crypto::digest_sha256::value_type a{ 0 };
			read_binary(is, a);
			return crypto::digest_sha256(a);
		}

		crypto::digest_sha512 reader_policy<crypto::digest_sha512>::extract(std::istream& is)
		{
			crypto::digest_sha512::value_type a{ 0 };
			read_binary(is, a);
			return crypto::digest_sha512(a);
		}

		boost::asio::ip::tcp::endpoint reader_policy<boost::asio::ip::tcp::endpoint>::extract(std::istream& is)
		{
			std::uint16_t port = read_binary<std::uint16_t>(is);
			std::istreambuf_iterator<char> eos;
			std::string address(std::istreambuf_iterator<char>(is), eos);
			boost::system::error_code ec;
			return boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(address, ec), port);
		}

		boost::asio::ip::udp::endpoint reader_policy<boost::asio::ip::udp::endpoint>::extract(std::istream& is)
		{
			std::uint16_t port = read_binary<std::uint16_t>(is);
			std::istreambuf_iterator<char> eos;
			std::string address(std::istreambuf_iterator<char>(is), eos);
			boost::system::error_code ec;
			return boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(address, ec), port);
		}

		boost::asio::ip::icmp::endpoint reader_policy<boost::asio::ip::icmp::endpoint>::extract(std::istream& is)
		{
			std::uint16_t port = read_binary<std::uint16_t>(is);
			std::istreambuf_iterator<char> eos;
			std::string address(std::istreambuf_iterator<char>(is), eos);
			boost::system::error_code ec;
			return boost::asio::ip::icmp::endpoint(boost::asio::ip::address::from_string(address, ec), port);
		}

		boost::asio::ip::address reader_policy<boost::asio::ip::address>::extract(std::istream& is)
		{
			std::istreambuf_iterator<char> eos;
			std::string address(std::istreambuf_iterator<char>(is), eos);
			boost::system::error_code ec;
			return boost::asio::ip::address::from_string(address, ec);
		}

		logging::severity reader_policy<logging::severity>::extract(std::istream& is)
		{
			return read_binary<logging::severity>(is);
		}

	}
}
