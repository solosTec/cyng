#include <cyng/task/stash.h>
#include <cyng/obj/clone.hpp>

#include <algorithm>
#include <iterator>

#include <boost/asio.hpp>
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
		if (!shutdown_) {
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
	}

	void stash::reset() {
		clear();
		shutdown_.exchange(false);
	}

	void stash::lock(channel_ptr scp) {
		if (!shutdown_ && scp) {
			dispatcher_.post([this, scp]() {
				if (!shutdown_)	list_.emplace(scp->get_id(), scp);
				});
		}
	}

	void stash::unlock(std::size_t id) {
		if (!shutdown_ && (id != 0u)) {
			dispatcher_.post([this, id]() {
				if (!shutdown_)	list_.erase(id);
				});
		}
	}

	void stash::unlock(std::string name) {
		if (!shutdown_ && !name.empty()) {
			dispatcher_.post([this, name]() {
				if (!shutdown_) {
					for (auto pos = list_.begin(); pos != list_.end(); ) {
						if (boost::algorithm::equals(pos->second->get_name(), name)) {
							pos = list_.erase(pos);
						}
						else {
							++pos;
						}
					}
				}
			});
		}
	}

	void stash::stop(std::string name) {
		if (!shutdown_ && !name.empty()) {
			dispatcher_.post([this, name]() {
				if (!shutdown_) {
					for (auto pos = list_.begin(); pos != list_.end(); ) {
						if (boost::algorithm::equals(pos->second->get_name(), name)) {
							auto channel = pos->second->shared_from_this();
							pos = list_.erase(pos);
							channel->stop();
						}
						else {
							++pos;
						}
					}
				}
			});
		}
	}

	std::size_t stash::size() const {
		return (!shutdown_) ? list_.size() : 0;
	}

}
