/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_OBJ_UTIL_HPP
#define DOCC_OBJ_UTIL_HPP

#include <cyng/obj/factory.hpp>
#include <cyng/obj/buffer_cast.hpp>

#include <boost/assert.hpp>

namespace docscript {

	/**
	 * Create an object from each parameter element and add
	 * it to a list of objects (tuple_t)
	 * 
	 * To not be confused with std::make_tuple() add the namespace docscript.
	 */
	template < typename ...Args >
	[[nodiscard]]
	tuple_t make_tuple(Args&&... args) {
		return { make_object<Args>(std::forward<Args>(args))... };
	}

	/**
	 * Convert all elements of an initializer list into a vector of objects.
	 */
	template < typename T >
	[[nodiscard]]
	vector_t make_vector(std::initializer_list<T> list)
	{
		vector_t vec;
		vec.reserve(list.size());
		std::transform(list.begin(), list.end(), std::back_inserter(vec), [](T const& v) {
			return make_object(v);
			});
		return vec;
	}

	/**
	 * Create an object from each parameter element and add
	 * it to a deque of objects (deque_t)
	 */
	template < typename ...Args >
	[[nodiscard]]
	deque_t make_deque(Args&&... args) {
		return { make_object<Args>(std::forward<Args>(args))... };
	}

	/**
	 * Create an attribute
	 */
	template < typename... Args >
	[[nodiscard]]
	attr_t make_attr(std::size_t idx, Args&&... args)
	{
		return { idx, make_object(std::forward<Args>(args)...) };
	}

	/**
	 * Create a parameter
	 */
	template < typename... Args >
	[[nodiscard]]
	param_t make_param(std::string const& name, Args&&... args)
	{
		BOOST_ASSERT_MSG(!name.empty(), "parameter without a name");
		return { name, make_object(std::forward<Args>(args)...) };
	}

	[[nodiscard]]
	std::string make_string(buffer_t const&);

	[[nodiscard]]
	std::string make_string(buffer_t const&, std::size_t offset);

	[[nodiscard]]
	std::filesystem::path make_fs_path(buffer_t const&);

	[[nodiscard]]
	mac48 make_mac48(buffer_t const&);

	[[nodiscard]]
	mac64 make_mac64(buffer_t const&);

	[[nodiscard]]
	pid make_pid(buffer_t const&);

	[[nodiscard]]
	obis make_obis(buffer_t const&);

	template <std::size_t N>
	[[nodiscard]]
	auto make_digest(buffer_t const& buffer) -> digest<N> {
		using type = digest<N>;
		if (buffer.size() >= type::size()) {
			typename type::digest_type a{ 0 };
			auto pos = std::begin(buffer);
			for (auto& e : a) {
				BOOST_ASSERT_MSG(pos != std::end(buffer), "buffer to small");
				e = *pos++;
			}
			return type(a);
		}
		return type{};
	}

	template <std::size_t N>
	auto make_aes_key(buffer_t const& buffer) -> aes_key<N> {
		using type = aes_key<N>;
		if (buffer.size() >= type::size()) {
			typename type::key_type a{ 0 };
			auto pos = std::begin(buffer);
			for (auto& e : a) {
				BOOST_ASSERT_MSG(pos != std::end(buffer), "buffer to small");
				e = *pos++;
			}
			return type(a);
		}
		return type{};
	}

	[[nodiscard]]
	boost::uuids::uuid make_uuid(buffer_t const&);

	[[nodiscard]]
	version make_version(buffer_t const&);

	[[nodiscard]]
	revision make_revision(buffer_t const&);

	[[nodiscard]]
	std::chrono::system_clock::time_point make_timepoint(buffer_t const&);

	template <typename T>
	[[nodiscard]]
	auto make_duration(buffer_t const& buffer) -> T {

		//std::chrono::duration<R, P>
		using R = typename T::rep;
		auto const rep = to_numeric<R>(buffer);
		return T(rep);
	}

	[[nodiscard]]
	boost::asio::ip::address make_ip_address(buffer_t const&);

	template<typename T>
	[[nodiscard]]
	auto make_endpoint(buffer_t const& buffer) -> T {

		// T => boost::asio::ip::basic_endpoint<T>;
		auto const port = to_numeric<std::uint16_t>(buffer);
		auto const address = make_string(buffer, sizeof(port));
		//	port + address
		return T(boost::asio::ip::make_address(address), port);
	}

	[[nodiscard]]
	boost::system::error_code make_ec(buffer_t const&);

	[[nodiscard]]
	op make_op(buffer_t const& buffer);

	[[nodiscard]]
	severity make_severity(buffer_t const& buffer);

}

#endif //	DOCC_OBJ_UTIL_HPP
