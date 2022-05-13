/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */

#ifndef CYNG_SQL_DSL_CONSTANT_HPP
#define CYNG_SQL_DSL_CONSTANT_HPP

#include <cyng/sql/dialect.h>
#include <cyng/store/meta.h>

#include <ostream>
#include <chrono>
#include <type_traits>

namespace cyng 
{
	template<class T>
	struct is_c_str
		: std::integral_constant<
		bool,
		std::is_same<char const*, typename std::decay<T>::type>::value ||
		std::is_same<char*, typename std::decay<T>::type>::value
		> {};

	/**
	 *	Simple placeholder for a constant value.
	 *	The problem with specialization for string literals popped up again :-(
	 */
	namespace sql
	{
		template < typename T >
		struct constant 
		{
			constant(T&& v)
				: value_(std::forward<T>(v))
			{}

			std::string to_str(dialect d, meta_sql const&) const {
				std::stringstream ss;
				ss << value_;
				return ss.str();
			}
						
			T const value_;
		};
		
		/**
		 * std::string
		 */
		template <>
		struct constant < std::string >
		{
			constant(std::string const& c);
			std::string to_str(dialect d, meta_sql const&) const;

			std::string const value_;
		};

		/**
		 * char array.
		 * Doesn't work so well with vc. The compiler deduces 
		 * the correct type as "const char[N]" but doesn't choose 
		 * this specialization. The "constant(T&& v)" constructors
		 * seems more appealing to it.
		 */
		template < std::size_t N >
		struct constant < const char[N] >
		{
			static_assert(N > 0, "N to small" );

			constant (const char(&c)[N])
				: value_(c, N - 1)	//	copy
			{}

			std::string to_str(dialect d, meta_sql const&) const {
				std::stringstream ss;
				ss << '\'' << value_ << '\'';
				return ss.str();
			}

			std::string const value_;
		};


		/**
		 * plain old const char*
		 */
		template <>
		struct constant < char const* >
		{
			constant(char const*);
			std::string to_str(dialect d, meta_sql const&) const;

			std::string const value_;
		};

		
		/**
		 * boolean
		 */
		template <>
		struct constant < bool >
		{
			constant(bool b);
			std::string to_str(dialect d, meta_sql const&) const;

			const bool b_;
		};
		
		/**
		 * timepoint
		 */
		template <>
		struct constant < std::chrono::system_clock::time_point >
		{
			constant(std::chrono::system_clock::time_point tp);			
			std::string to_str(dialect d, meta_sql const&) const;

			const std::chrono::system_clock::time_point tp_;
		};

		template < typename T >
		inline constant< T > make_constant(T&& v) {
			return constant<T>(std::forward<T>(v));
		}

		/**
		 * specialized for C-style strings
		 */
		template < std::size_t N >
		inline constant< std::string > make_constant(const char(&c)[N]) {
			return constant<std::string>(std::string(c, N - 1));
		}

	}	
}

#endif	//	CYNG_SQL_DSL_CONSTANT_HPP




