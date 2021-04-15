/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_CSV_LINE_CAST_HPP
#define CYNG_CSV_LINE_CAST_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)


#include <cyng/parse/csv/csv_parser.h>
#include <cyng/parse/string.h>

#include <boost/algorithm/string.hpp>

#include <tuple>

namespace cyng	
{
	namespace csv 
	{
		namespace {

			//
			//	numerics
			//
			template <typename T>
			struct policy
			{
				static T cast(std::string const& s) {
					return to_numeric<T>(s);
				}
			};

			//
			//	strings
			//
			template <>
			struct policy<std::string>
			{
				static std::string cast(std::string const& s) {
					return s;
				}
			};

			//
			//	bool
			//
			template <>
			struct policy<bool>
			{
				static bool cast(std::string const& s) {
					return boost::algorithm::equals(s, "true");
				}
			};

			//
			//	aes key
			//
			template <std::size_t N>
			struct policy<aes_key<N>>
			{
				static aes_key<N> cast(std::string const& str) {
					BOOST_ASSERT_MSG(str.size() == N / 4, "invalid AES key format");
					return (str.size() == N / 4)
						? to_aes_key<N>(str)
						: aes_key<N>()
						;
				}
			};

			//
			//	time point
			//
			template <>
			struct policy<std::chrono::system_clock::time_point>
			{
				static std::chrono::system_clock::time_point cast(std::string const& str) {
					return to_tp_iso8601(str);
				}
			};

			//
			//	UUID
			//
			template <>
			struct policy<boost::uuids::uuid>
			{
				static boost::uuids::uuid cast(std::string const& str) {
					return to_uuid(str);
				}
			};

			template < std::size_t N, typename ...Args >
			struct convert
			{
				using tuple_t = typename std::tuple<Args...>;
				using U = typename std::tuple_element<N, tuple_t>::type;
				using iter_t = typename line_t::const_reverse_iterator;

				static void set(iter_t pos, iter_t end, tuple_t& out)
				{
#ifdef _DEBUG
					std::cerr << "N: " << N << " - " << *pos << std::endl;
#endif
					if (pos != end) {
						std::get< N >(out) = policy<U>::cast(*pos);
						//	recursion
						convert<N - 1, Args...>::set(++pos, end, out);
					}
				}
			};

			//	terminate recursion 
			template < typename ...Args >
			struct convert < 0, Args... >
			{
				using tuple_t = typename std::tuple<Args...>;
				using U = typename std::tuple_element<0, tuple_t>::type;
				using iter_t = typename line_t::const_reverse_iterator;

				static void set(iter_t pos, iter_t end, std::tuple<Args...>& out)
				{
#ifdef _DEBUG
					std::cerr << "N: 0 - " << *pos << std::endl;
#endif
					BOOST_ASSERT(pos != end);
					std::get< 0 >(out) = policy<U>::cast(*pos);

				}
			};

			template < typename ... Args >
			struct line_cast_helper
			{
				using size = std::integral_constant<std::size_t, sizeof...(Args)>;
				using R = typename std::tuple< Args... >;

				using cast_type = convert< size::value - 1, Args...>;

				static R convert(line_t const& line)
				{
					//	compile time meets runtime
					using size = std::integral_constant<std::size_t, sizeof...(Args)>;
					BOOST_ASSERT(size::value == line.size());

					R out;
					cast_type::set(line.crbegin(), line.crend(), out);
					return out;
				}
			};

		}

		/**
		 * convert a vector of string into a tuple of the specified types
		 */
		template <typename ...Args>
		auto line_cast(line_t const& line) -> std::tuple<Args...> {

			constexpr std::size_t tpl_size = sizeof...(Args);
			BOOST_ASSERT(line.size() == tpl_size);
			if (line.size() == tpl_size) {
				return line_cast_helper<Args...>::convert(line);
			}

			return std::tuple<Args...>();
		}

	}
}

#endif

