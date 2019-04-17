/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_CRYPTO_RND_H
#define CYNG_CRYPTO_RND_H


#include <string>
#include <ctime>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>

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

			std::string next(std::size_t);
			std::string operator()(std::size_t);

		private:
			const std::string stock_;
			boost::random::random_device rng_;
			boost::random::uniform_int_distribution<std::size_t> index_dist_;
		};

		template <typename T>
		class rnd_num
		{
		public:
			rnd_num(T min_arg, T max_arg)
				: rng_()
				, monitor_dist_(min_arg, max_arg)
			{
				rng_.seed(static_cast<std::uint32_t>(std::time(nullptr)));
			}

			T next()
			{
				return monitor_dist_(rng_);
			}

			T operator()()
			{
				return next();
			}

		private:
			boost::random::mt19937 rng_;
			boost::random::uniform_int_distribution<T> monitor_dist_;
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

