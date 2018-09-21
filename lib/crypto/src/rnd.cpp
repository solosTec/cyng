/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include <cyng/crypto/rnd.h>

namespace cyng
{
	namespace crypto 
	{
		rnd::rnd(std::string const& stock)
			: stock_(stock)
			, rng_()
			, index_dist_(0, stock_.size() - 1u)
		{}

		rnd::rnd(rnd const& other)
			: stock_(other.stock_)
			, rng_()
			, index_dist_(0, stock_.size() - 1u)
		{}

		std::string rnd::next(std::size_t size)
		{
			std::string r;
			r.reserve(size);
			while (size-- > 0) {
				r += stock_.at(index_dist_(rng_));
			}
			return r;
		}

		std::string rnd::operator()(std::size_t size)
		{
			return next(size);
		}

		rnd make_rnd_alnum()
		{
			return rnd("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
		}

		rnd make_rnd_num()
		{
			return rnd("0123456789");
		}

	}
}

