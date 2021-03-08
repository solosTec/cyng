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
		, shutdown_(false)
	{}

	void registry::insert(task_interface* p)
	{
		if (shutdown_)	return;
		dispatcher_.post([this, p]() {
			list_.emplace(p->get_id(), p);
		});
	}

	void registry::remove_sync(std::size_t id, destruct_cb cb) {
		auto pos = list_.find(id);

		if (pos != list_.end()) {
			auto* p = pos->second.release();
			list_.erase(pos);

			//
			//	channel callback
			//
			cb(p);
		}
	}

	void registry::remove(std::size_t id, destruct_cb cb)
	{
		if (shutdown_) {
			remove_sync(id, cb);
		}
		else {
			dispatcher_.post([this, id, cb]() {

				remove_sync(id, cb);
				});
		}
	}

	channel_ptr registry::lookup_sync(std::size_t id)
	{
		auto pos = list_.find(id);
		return (pos != list_.end())
			? pos->second->get_channel()
			: channel_ptr()
			;
	}

	std::vector<channel_ptr> registry::lookup_sync(std::string name)
	{
		std::vector<channel_ptr> channels;
		for (auto const& ch : list_) {
			auto sp = ch.second->get_channel();
			if (sp && boost::algorithm::equals(name, sp->get_name())) {
				channels.push_back(sp);
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

	std::vector<channel_ptr> registry::get_all_channels() const
	{
		//
		//	shutdown mode
		//
		BOOST_ASSERT(shutdown_);

		std::vector<channel_ptr> vec;

		vec.reserve(list_.size());
		std::transform(std::begin(list_), std::end(list_), std::back_inserter(vec), [](list_t::value_type const& val) {
			return val.second->get_channel();
			});
		return vec;
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
			auto channels = get_all_channels();

			//
			//	stop all channels
			//
			for (auto ptr : channels) {
				ptr->shutdown();
			}

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

}
