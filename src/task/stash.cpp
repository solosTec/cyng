#include <cyng/task/stash.h>
#include <cyng/obj/clone.hpp>

#include <algorithm>
#include <iterator>

#include <thread>
#include <future>
#include <boost/asio.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>

namespace cyng {

	stash::stash(boost::asio::io_context& io)
		: dispatcher_(io)
		, list_()
		, shutdown_(false)
	{}

	void stash::clear() {
		if (!shutdown_.exchange(true)) {
			list_.clear();
		}
	}

	void stash::stop() {
		for (auto pos = list_.rbegin(); pos != list_.rend(); pos++) {
			auto scp = pos->second;
			if (scp) {
#ifdef _DEBUG
				auto const name = scp->get_name();
				auto const id = scp->get_id();
#endif
				scp->stop();
			}
		}

	}

	void stash::lock(channel_ptr scp) {
		if (!shutdown_ && scp) {
			dispatcher_.post([this, scp]() {
				list_.emplace(scp->get_id(), scp);
				});
		}
	}

	channel_ptr stash::unlock(std::size_t id) {
		return (!shutdown_)
			? find_channel(id, boost::asio::use_future).get()
			: channel_ptr();
	}
}
