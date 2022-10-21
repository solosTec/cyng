/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_VM_LINEARIZE_HPP
#define CYNG_VM_LINEARIZE_HPP

#include <cyng/obj/util.hpp>
#include <cyng/obj/buffer_cast.hpp>

#ifdef _DEBUG_VM
#include <cyng/io/ostream.h>
#include <iostream>
#endif

#include <tuple>
#include <deque>
#include <type_traits>

#include <boost/asio/streambuf.hpp>

namespace cyng {

	namespace {

		template <typename T, typename Enable = void >
		struct linearize_policy;

		template <typename T>
		struct linearize_policy<T, typename std::enable_if<std::is_integral<T>::value>::type>
		{
			static buffer_t linearize(T val) {
				return to_buffer<T>(val);
			}
		};
			
		template <typename T>
		struct linearize_policy<T, typename std::enable_if<std::is_floating_point<T>::value>::type>
		{
			static buffer_t linearize(T val) {
				return to_buffer<T>(val);
			}
		};

		template <>
		struct linearize_policy<std::string>
		{
			static buffer_t linearize(std::string val) {
				return make_buffer(val);
			}
		};

		template <typename T>
		buffer_t linearize_impl(T v) {
			using value_type = std::decay_t<T>;
			return linearize_policy< value_type >::linearize(v);
		}

	}
	/**
	 * take a list of values and serialize each element into a simple
	 * character buffer. Each buffer will be added to a deque.
	 */
	template < typename ...Args >
	std::deque<buffer_t> linearize(Args&&... args) {
		return { linearize_impl<Args>(args)... };
	}

	/**
	 * Convert an object to a T-L-V binary representation.
	 * This function uses internally streams and is probably slow.
	 */
	buffer_t convert_binary(object const& obj);

	namespace {

		template <typename T>
		buffer_t convert_impl(T val) {
			auto const obj = make_object(val);
			return convert_binary(obj);
		}
	}

	/**
	 * This is inherently slow
	 */
	template < typename ...Args >
	std::deque<buffer_t> convert_to_deque(Args&&... args) {
		return { convert_impl<Args>(args)... };
	}


}
#endif

