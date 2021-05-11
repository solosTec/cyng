#include <cyng/task/registry.h>
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

	registry::registry(boost::asio::io_context& io)
		: dispatcher_(io)
		, list_()
		, locked_()
		, shutdown_(false)
	{}

	void registry::insert(channel_ptr chp)
	{
		if (shutdown_)	return;
		dispatcher_.post([this, chp]() {
			list_.emplace(chp->get_id(), chp);
		});
	}

	void registry::remove_sync(std::size_t id) {
		auto pos = list_.find(id);

		if (pos != list_.end()) {
			//
			//	remove from list and delete task
			//
			try {
				list_.erase(pos);
			}
			catch (...) {}
		}
	}

	void registry::remove(std::size_t id)
	{
		if (!shutdown_) {
			dispatcher_.post([this, id]() {

				remove_sync(id);
			});
		}
		//else {
		//	remove_sync(id);
		//}
	}

	channel_ptr registry::lookup_sync(std::size_t id)
	{
		auto pos = list_.find(id);
		return (pos != list_.end())
			? pos->second.lock()	
			: channel_ptr()
			;
	}

	std::vector<channel_ptr> registry::lookup_sync(std::string name)
	{
		std::vector<channel_ptr> channels;
		for (auto const& wcp : list_) {
			auto scp = wcp.second.lock();
			if (scp && boost::algorithm::equals(name, scp->get_name())) {
				channels.push_back(scp);
			}
		}
		return channels;
	}

	channel_ptr registry::lookup(std::size_t id)
	{
 		// static_assert(BOOST_ASIO_HAS_STD_FUTURE == 1, "asio future not supported");
		if (!shutdown_) {

			auto answer = find_channel(id, boost::asio::use_future);
			return answer.get();
		}

		return channel_ptr();
	}

	std::vector<channel_ptr> registry::lookup(std::string name)
	{
		if (!shutdown_) {

			auto answer = find_channels(name, boost::asio::use_future);
			return answer.get();
		}

		return std::vector<channel_ptr>();
	}

	bool registry::shutdown()
	{
		if (!shutdown_.exchange(true))	{

			//
			//	registry no longer available
			//

			//
			//	get all channels
			//
			auto channels = get_all_channels(list_);

			//
			//	stop all channels
			//
			for (auto ptr : channels) {
				if (ptr) {
#ifdef _DEBUG
					auto const name = ptr->get_name();
					auto const id = ptr->get_id();
#endif
					ptr->stop();
				}
			}

			//
			//	remove references
			//
			channels.clear();

			//
			//	clear channel table
			//
			list_.clear();

			//
			//	clear lock table
			//
			locked_.clear();

			return true;
		}
		return false;
	}

	bool registry::reset()
	{
		//
		//	write lock
		//
		BOOST_ASSERT(list_.empty());
		return (shutdown_.exchange(false) && list_.empty());
	}

	std::size_t registry::dispatch(std::string channel, std::string slot, tuple_t&& msg) {
		auto channels = lookup(channel);
		for (auto& chp : channels) {
			//	since every dispatch call expects it's own copy
			//	of the data, we have to clone it.
			chp->dispatch(slot, clone(msg));
		}
		return channels.size();
	}

	void registry::lock(channel_ptr scp) {
		if (!shutdown_ && scp) {
			dispatcher_.post([this, scp]() {
				locked_.emplace(scp->get_id(), scp);
				});
		}
	}

	channel_ptr registry::unlock(std::size_t id) {
		return (!shutdown_)
			? find_locked_channel(id, boost::asio::use_future).get()
			: channel_ptr();
	}

	std::vector<channel_ptr> get_all_channels(registry::list_t const& reg_list)
	{
		//
		//	shutdown mode
		//

		std::vector<channel_ptr> vec;

		vec.reserve(reg_list.size());
		std::transform(std::begin(reg_list), std::end(reg_list), std::back_inserter(vec), [](registry::list_t::value_type const& val) {
			return val.second.lock();
			});
		return vec;
	}

	auto_remove::auto_remove(registry& reg, std::size_t id)
		: reg_(reg)
		, id_(id)
	{}

	void auto_remove::operator ()(channel* cp) 	{
#ifdef _DEBUG
		if (cp->is_open())	BOOST_ASSERT(cp->get_id() == id_);
#endif
		reg_.remove(id_);
		delete cp;
	}

}
