#include <cyng/task/registry.h>

#include <cyng/obj/clone.hpp>

#include <algorithm>
#include <iterator>

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/assert.hpp>

namespace cyng {

    registry::registry(boost::asio::io_context &io)
        : dispatcher_(io)
        , list_()
#ifdef _DEBUG
        , on_stop_()
#endif
        , shutdown_(false) {
    }

#ifdef _DEBUG
    registry::registry(boost::asio::io_context &io, std::function<void(std::string, std::size_t)> on_stop)
        : dispatcher_(io)
        , list_()
        , on_stop_(on_stop)
        , shutdown_(false) {}
#endif

    void registry::insert(channel_ptr chp) {
        if (shutdown_)
            return;
        dispatcher_.post([this, chp]() {
            auto const id = chp->get_id();
            BOOST_ASSERT(id != 0u);
            list_.emplace(id, chp);
        });
    }

    void registry::remove_sync(std::size_t id) {
        auto pos = list_.find(id);

        if (pos != list_.end()) {
            //
            //	remove from list and delete task
            //
            try {
#ifdef _DEBUG
                if (on_stop_) {
                    auto channel = pos->second.lock();
                    on_stop_((channel) ? channel->get_name() : "", pos->first);
                }
#endif
                list_.erase(pos);
            } catch (...) {
            }
        }
    }

    void registry::remove(std::size_t id) {
        if (!shutdown_) {
            dispatcher_.post([this, id]() { remove_sync(id); });
        }
    }

    channel_ptr registry::lookup_sync(std::size_t id) {
        auto pos = list_.find(id);
        return (pos != list_.end()) ? pos->second.lock() : channel_ptr();
    }

    std::vector<channel_ptr> registry::lookup_sync(std::string name) {
        std::vector<channel_ptr> channels;
        for (auto const &wcp : list_) {
            auto scp = wcp.second.lock();
            if (scp && boost::algorithm::equals(name, scp->get_name())) {
                channels.push_back(scp);
            }
        }
        return channels;
    }

    void registry::lookup(std::string name, std::function<void(std::vector<channel_ptr>)> cb) {
        dispatcher_.post([this, cb, name]() mutable { cb(lookup_sync(name)); });
    }

    bool registry::shutdown() {
        if (!shutdown_.exchange(true)) {

            //
            //	registry no longer available
            //

            //
            //	stop all channels (reverse)
            //
            for (auto pos = list_.rbegin(); pos != list_.rend(); pos++) {
                auto scp = pos->second.lock();
                if (scp) {
#ifdef _DEBUG
                    auto const name = scp->get_name();
                    auto const id = scp->get_id();
                    if (on_stop_) {
                        on_stop_(name, id);
                    }
#endif
                    scp->stop();
                }
            }

            //
            //	clear channel table
            //
            list_.clear();

            return true;
        }
        return false;
    }

    bool registry::is_shutdown() const { return shutdown_.load(); }

    bool registry::reset() {
        //
        //	write lock
        //
        BOOST_ASSERT(list_.empty());
        return (shutdown_.exchange(false) && list_.empty());
    }

    void registry::dispatch(std::vector<std::size_t> tasks, std::string slot, channel::cb_err_t cb, tuple_t &&msg) {
        if (!shutdown_) {
            dispatcher_.post([=, this]() {
                for (auto const id : tasks) {
                    auto const pos = list_.find(id);
                    if (pos != list_.end()) {
                        auto sp = pos->second.lock();
                        if (sp) {
                            //  since the msg is "moved" a copy of each message is reqired
                            sp->dispatch(slot, cb, clone(msg));
                        }
                    }
                }
            });
        }
    }

    void registry::dispatch(std::string channel, std::string slot, channel::cb_err_t cb, tuple_t msg) {

        if (!shutdown_) {
            dispatcher_.post([=, this]() mutable {
                auto channels = lookup_sync(channel);
                for (auto &chp : channels) {
                    //	since every dispatch call expects it's own copy
                    //	of the data, we have to clone it.
                    chp->dispatch(slot, cb, clone(msg));
                }
            });
        }
    }

    void registry::dispatch(std::size_t channel, std::string slot, channel::cb_err_t cb, tuple_t msg) {
        if (!shutdown_) {
            dispatcher_.post([=, this]() mutable {
                auto pos = list_.find(channel);
                if (pos != list_.end()) {
                    auto sp = pos->second.lock();
                    if (sp) {
                        sp->dispatch(slot, cb, std::move(msg));
                    }
                }
            });
        }
    }

    void registry::dispatch_exclude(std::size_t id, std::string name, std::string slot, channel::cb_err_t cb, tuple_t &&msg) {

        lookup(name, [=, this](std::vector<channel_ptr> channels) {
            for (auto &source : channels) {

                //
                //	exclude
                //
                if (source->get_id() != id) {

                    //
                    //	since every dispatch call expects it's own copy
                    //	of the data, we have to clone it.
                    source->dispatch(slot, cb, clone(msg));
                }
            }
        });
    }

    void registry::dispatch_exclude(channel_ptr channel, std::string slot, channel::cb_err_t cb, tuple_t &&msg) {
        if (channel) {
            dispatch_exclude(channel->get_id(), channel->get_name(), slot, cb, std::move(msg));
        }
    }

    void registry::stop(std::string name) {
        lookup(name, [](std::vector<cyng::channel_ptr> channels) {
            for (auto receiver : channels) {
                receiver->stop();
            }
        });
    }

    auto_remove::auto_remove(registry &reg, std::size_t id)
        : reg_(reg)
        , id_(id) {}

    void auto_remove::operator()(channel *cp) {

        BOOST_ASSERT_MSG(!cp->is_open(), "channel still open");
        reg_.remove(id_);
        delete cp;
    }

} // namespace cyng
