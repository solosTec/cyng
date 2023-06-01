#include <cyng/obj/object.h>
#include <cyng/task/channel.h>
#include <cyng/task/task.hpp>

#if defined _DEBUG_TASK || defined _DEBUG
#include <boost/algorithm/string.hpp>
#include <cyng/io/ostream.h>
#include <iostream>
#include <string>
#endif

#include <boost/assert.hpp>

namespace cyng {

    channel::channel(boost::asio::io_context &io, std::string name)
        : std::enable_shared_from_this<channel>()
        , slot_names()
        , dispatcher_(io)
        , timer_(io)
        , task_(nullptr)
        , name_(name) {}

    void channel::dispatch(std::size_t slot, tuple_t &&msg) {
        if (is_open(slot)) {

            //
            //  the context should be active
            //
            BOOST_ASSERT(!dispatcher_.context().stopped());
            BOOST_ASSERT_MSG(slot != std::numeric_limits<std::size_t>::max(), "unknown slot name");

            //
            //	thread safe access to task
            //
            auto sp = shared_from_this(); //  extend life time

            boost::asio::post(dispatcher_, [this, sp, slot, msg]() -> void {
                if (sp->is_open(slot)) {
                    auto const tpl = task_->dispatch(slot, msg, sp);
                    boost::ignore_unused(tpl);
                }
            });
        }
    }

    void channel::dispatch_r(std::size_t slot, tuple_t &&msg, std::function<void(cyng::tuple_t)> cb) {
        if (is_open(slot)) {

            //
            //  the context should be active
            //
            BOOST_ASSERT(!dispatcher_.context().stopped());
            BOOST_ASSERT_MSG(slot != std::numeric_limits<std::size_t>::max(), "unknown slot name");

            //
            //	thread safe access to task
            //
            auto sp = shared_from_this(); //  extend life time

            boost::asio::post(dispatcher_, [this, sp, slot, msg, cb]() -> void {
                if (sp->is_open(slot)) {
                    cb(task_->dispatch(slot, msg, sp));
                }
            });
        }
    }

    void channel::dispatch(std::size_t slot) { dispatch(slot, make_tuple()); }

    void channel::dispatch(std::string name, cb_err_t cb, tuple_t &&msg) {
        auto const [slot, ok] = lookup(name);
        if (ok) {
            dispatch(slot, std::move(msg));
        } else {
            //  error case: slot name is not defined
            if (cb) {
                cb(name_, name);
            }
        }
    }

    void channel::dispatch(std::string name, cb_err_t cb) { dispatch(name, cb, {}); }

    void channel::next(std::size_t slot_producer, std::size_t slot_consumer, tuple_t &&msg) {
        next(
            slot_producer,
            [&, this](cyng::tuple_t &&msg) -> void { this->dispatch(slot_consumer, std::move(msg)); },
            std::move(msg));
    }

    void channel::next(std::string slot_producer, std::string slot_consumer, cb_err_t cb, tuple_t &&msg) {
        next(
            slot_producer,
            cb,
            [=, this](cyng::tuple_t &&msg) -> void {
                // void dispatch(std::string slot, cb_err_t cb, tuple_t &&msg);
                this->dispatch(slot_consumer, cb, std::move(msg));
            },
            std::move(msg));
    }

    void channel::next(std::size_t slot_producer, channel_ptr consumer, std::size_t slot_consumer, tuple_t &&msg) {
        next(
            slot_producer,
            [&, this](cyng::tuple_t &&msg) -> void { consumer->dispatch(slot_consumer, std::move(msg)); },
            std::move(msg));
    }

    void channel::next(std::size_t slot, std::function<void(tuple_t &&msg)> f, tuple_t &&msg) {
        if (is_open(slot)) {

            //
            //  the context should be active
            //
            BOOST_ASSERT(!dispatcher_.context().stopped());
            BOOST_ASSERT_MSG(slot != std::numeric_limits<std::size_t>::max(), "unknown slot name");

            //
            //	thread safe access to task
            //
            auto sp = shared_from_this(); //  extend life time

            boost::asio::post(dispatcher_, [this, sp, slot, f, msg]() -> void {
                if (sp->is_open(slot))
                    task_->next(slot, f, msg);
            });
        }
    }

    void channel::next(std::string name, cb_err_t cb, std::function<void(tuple_t &&msg)> f, tuple_t &&msg) {
        auto const [slot, ok] = lookup(name);
        if (ok) {
            next(slot, f, std::move(msg));
        } else {
            if (cb) {
                cb(name_, name);
            }
        }
    }

    bool channel::is_open() const noexcept { return task_.operator bool(); }

    bool channel::is_open(std::size_t slot) const noexcept {
        BOOST_ASSERT_MSG(slot < task_->get_signature_count(), "slot out of range");

        return is_open() && (slot < task_->get_signature_count());
    }

    bool channel::stop() {
        //  extend life time
        auto sp = this->shared_from_this();

        //
        //  mark channel as closed
        //	release pointer so that the task object can control its own life time
        //
        auto ptr = task_.release();

        //
        //  already closed (this is safe)
        //
        if (nullptr == ptr)
            return false;
        BOOST_ASSERT(!is_open());

        //
        //  cancel timer
        //
        timer_.cancel();
        dispatcher_.post([this, ptr, sp]() mutable {
            //
            //  call stop(eod) in task implementation class
            //
            ptr->stop(sp);
        });

        ptr = nullptr;
        return true;
    }

    bool channel::cancel_timer() { return timer_.cancel() > 0; }

    std::string const &channel::get_name() const noexcept { return name_; }

    std::size_t channel::get_id() const noexcept { return (is_open()) ? task_->get_id() : 0; }

    bool channel::suspend(time_point_t tp, std::size_t slot, tuple_t &&msg) {

        if (!is_open(slot))
            return false;

        auto sp = this->shared_from_this(); //  extend life time

        timer_.expires_at(tp);

        timer_.async_wait(boost::asio::bind_executor(dispatcher_, [this, slot, msg, sp](boost::system::error_code const &ec) {
            if (ec != boost::asio::error::operation_aborted && sp->is_open(slot)) {
                task_->dispatch(slot, msg, sp);
            }
        }));

        return true;
    }

    bool channel::suspend(time_point_t tp, std::string name, cb_err_t cb, tuple_t &&msg) {
        auto const [slot, ok] = lookup(name);
        if (ok) {
            return suspend(tp, slot, std::move(msg));
        }
        //  error
        if (cb) {
            cb(name_, name);
        }
        return false;
    }

    channel::result::result(channel &c, std::size_t slot, tuple_t &&msg)
        : ready_(false)
        , result_() {
        c.dispatch_r(slot, std::move(msg), [this](cyng::tuple_t tpl) -> void {
            ready_ = true;
            result_ = tpl;
        });
    }

    bool channel::result::is_ready() const { return ready_; }
    std::pair<cyng::tuple_t, bool> channel::result::get_result() const { return {result_, is_ready()}; }

    boost::asio::io_context::strand &expose_dispatcher(channel &ch) {
        BOOST_ASSERT(ch.is_open());
        return ch.dispatcher_;
    }

    slot_names::slot_names()
        : named_slots_() {}

    /**
     * Specify the name of a channel
     */
    bool slot_names::set_channel_name(std::string name, std::size_t slot) { return named_slots_.emplace(name, slot).second; }

    void slot_names::set_channel_names(std::initializer_list<std::string> il) {
        std::size_t index{named_slots_.size()};
        for (auto const &name : il) {
            set_channel_name(name, index);
            ++index;
        }
    }

    std::pair<std::size_t, bool> slot_names::lookup(std::string const &name) const {
        auto const pos = named_slots_.find(name);
        return (pos != named_slots_.end()) ? std::make_pair(pos->second, true)
                                           : std::make_pair(std::numeric_limits<std::size_t>::max(), false);
    }

} // namespace cyng

namespace std {

    size_t hash<cyng::channel_ptr>::operator()(cyng::channel_ptr v) const noexcept { return (v) ? v->get_id() : 0u; }
    size_t hash<cyng::channel_weak>::operator()(cyng::channel_weak v) const noexcept {
        auto channel = v.lock();
        return (channel) ? hash<cyng::channel_ptr>()(channel) : 0;
    }
} // namespace std
