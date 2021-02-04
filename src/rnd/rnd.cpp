/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher
 *
 */ 

#include <cyng/rnd/rnd.hpp>

namespace cyng
{
	namespace crypto 
	{
		rnd::rnd(std::string const& stock)
			: stock_(stock)
#if BOOST_OS_WINDOWS
			, rnd_()
			, gen_(rnd_())
#else
			, gen_(std::time(0))
#endif
			, index_dist_(0, stock_.size() - 1u)
		{}

		rnd::rnd(rnd const& other)
			: stock_(other.stock_)
#if BOOST_OS_WINDOWS
			, rnd_()
			, gen_(rnd_())
#else
			, gen_(std::time(0))
#endif
			, index_dist_(0, stock_.size() - 1u)
		{}

		char rnd::next()
		{
			return stock_.at(index_dist_(gen_));
		}

		std::string rnd::next(std::size_t size)
		{
			std::string r;
			r.reserve(size);
			while (size-- > 0) {
				r += next();
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

		rnd make_rnd_hex()
		{
			return rnd("0123456789ABCDEF");
		}

		rnd make_rnd_pwd()
		{
			return rnd("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-=!?+@#%/()[]$");
		}

	}
}

