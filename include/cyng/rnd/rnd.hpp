/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#ifndef CYNG_RND_HPP
#define CYNG_RND_HPP


#include <string>
#include <ctime>

#include <random>
#include <boost/predef.h>

namespace cyng 
{
	namespace crypto 
	{
		/**
		 * Simple class to generate random strings with the same character set and different length.
		 */
		class rnd
		{
		public:
			rnd(std::string const&);
			rnd(rnd const&);

			char next();
			std::string next(std::size_t);
			std::string operator()(std::size_t);

		private:
			std::string const stock_;
#if defined(BOOST_OS_WINDOWS_AVAILABLE)
			std::random_device rnd_;
			std::mt19937 gen_;
#else
			std::mt19937 gen_;
#endif
			std::uniform_int_distribution<std::size_t> index_dist_;

		};

		template <typename T>
		class rnd_num
		{
		public:
			rnd_num(T min_arg, T max_arg)
#if defined(BOOST_OS_WINDOWS_AVAILABLE)
				: rnd_()
				, gen_(rnd_())
#else
				: gen_(std::time(0))
#endif
				, monitor_dist_(min_arg, max_arg)
			{
				// rng_.seed(static_cast<std::uint32_t>(std::time(nullptr)));
			}

			T next()
			{
				return monitor_dist_(gen_);
			}

			T operator()()
			{
				return next();
			}

		private:
#if defined(BOOST_OS_WINDOWS_AVAILABLE)
			std::random_device rnd_;
			std::mt19937 gen_;
#else
			std::mt19937 gen_;
#endif
			std::uniform_int_distribution<T> monitor_dist_;
		};

		/**
		 * Generator for alphanumeric strings
		 */
		rnd make_rnd_alnum();

		/**
		 * Generator for numeric strings
		 */
		rnd make_rnd_num();

		/**
		 * Generator for hexadecimal strings
		 */
		rnd make_rnd_hex();

		/**
		 * Generator for password strings
		 * including all latin characters, all numbers 
		 * and special characters -=!?+@#%/()[]$
		 */
		rnd make_rnd_pwd();

		/**
		 * Generator for integers
		 */
		template <typename T>
		auto make_rnd(T min_arg, T max_arg) -> rnd_num<T>
		{
			return rnd_num<T>(min_arg, max_arg);
		}
	}
	
}

#endif	

