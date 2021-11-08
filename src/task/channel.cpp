#include <cyng/task/channel.h>
#include <cyng/task/task.hpp>
#include <cyng/obj/object.h>

#ifdef _DEBUG_TASK
#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>
#include <cyng/io/ostream.h>
#endif

#include <boost/assert.hpp>

namespace cyng {


    channel::channel(boost::asio::io_context& io, std::string name)
        : std::enable_shared_from_this<channel>()
        , slot_names()
        , dispatcher_(io)
        , timer_(io)
        , task_(nullptr)
        , name_(name)
    {}

    void channel::dispatch(std::size_t slot, tuple_t&& msg)
    {
        if (is_open(slot)) {

            //
            //  the context should be active
            //
            BOOST_ASSERT(!dispatcher_.context().stopped());
            BOOST_ASSERT_MSG(slot != std::numeric_limits<std::size_t>::max(), "unknown slot name");

            //
            //	thread safe access to task
            //
            message m(this->shared_from_this(), slot, std::move(msg));
            auto sp = this->shared_from_this(); //  extend life time

            boost::asio::post(dispatcher_, [this, sp, m]()->void {
                if (is_open(m.slot_))  task_->dispatch(m.slot_, m.msg_);
             });
        }
    }

    void channel::dispatch(std::size_t slot) {
        dispatch(slot, make_tuple());
    }

    void channel::dispatch(std::string slot, tuple_t&& msg) {
        dispatch(lookup(slot), std::move(msg));
    }

    void channel::dispatch(std::string slot) {
        dispatch(lookup(slot), make_tuple());
    }

    bool channel::is_open() const noexcept {
        return task_.operator bool();
    }

    bool channel::is_open(std::size_t slot) const noexcept
    {
        BOOST_ASSERT_MSG(slot < task_->get_signature_count(), "slot out of range");

        return is_open()
            && (slot < task_->get_signature_count())
            ;
    }

    void channel::stop()
    {
        //  extend life time
        auto sp = this->shared_from_this(); 

        //
        //  mark channel as closed
        //	release pointer so that the task object can control its own life time
        //
        auto ptr = task_.release();

        if (nullptr == ptr) return;
        BOOST_ASSERT(!is_open());

        //
        //  cancel timer
        //
        timer_.cancel();
        dispatcher_.post([this, ptr, sp]() mutable {

            //
            //  call stop(eod) in task implementation class
            //
            ptr->stop();
        });

        ptr = nullptr;
    }

    bool channel::cancel_timer() {
        return timer_.cancel() > 0;
    }

    std::string const& channel::get_name() const noexcept
    {
        return name_;
    }

    std::size_t channel::get_id() const noexcept
    {
        return (is_open())
            ? task_->get_id()
            : 0
            ;
    }

    void channel::suspend(time_point_t tp, std::size_t slot, tuple_t&& msg) {

        if (!is_open(slot))	return;

        message m(this->shared_from_this(), slot, std::move(msg));
        auto sp = this->shared_from_this(); //  extend life time

        timer_.expires_at(tp);
        //timer_.expires_at(std::chrono::time_point_cast<std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>>(tp));

        timer_.async_wait(boost::asio::bind_executor(dispatcher_, [this, sp, m](boost::system::error_code const& ec) {
            if (ec != boost::asio::error::operation_aborted && is_open(m.slot_)) {
                task_->dispatch(m.slot_, m.msg_);
            }
            }));
    }

    void channel::suspend(time_point_t tp, std::string slot, tuple_t&& msg) {
        suspend(tp, lookup(slot), std::move(msg));
    }


    boost::asio::io_context::strand& expose_dispatcher(channel& cr) {
        return cr.dispatcher_;
    }

    message::message(channel_ptr cp, std::size_t id, tuple_t&& msg)
        : cp_(cp)
        , slot_(id)
        , msg_(std::move(msg))  //  move
    {}
    message::message(channel_ptr cp, std::size_t id, tuple_t const& msg)
        : cp_(cp)
        , slot_(id)
        , msg_(msg) //  copy
    {}

    message::message(message const& msg)
        : cp_(msg.cp_)
        , slot_(msg.slot_)
        , msg_(msg.msg_)  //  copy
    {}

    message::message(message&& msg) noexcept
        : cp_(std::move(msg.cp_))
        , slot_(msg.slot_)
        , msg_(std::move(msg.msg_))  //  move
    {}

    slot_names::slot_names()
        : named_slots_()
    {}

    /**
     * Specify the name of a channel
     */
    bool slot_names::set_channel_name(std::string name, std::size_t slot) {
        return named_slots_.emplace(name, slot).second;
    }

    void slot_names::set_channel_names(std::initializer_list<std::string> il) {
        std::size_t index{ named_slots_.size() };
        for (auto pos = il.begin(); pos != il.end(); ++pos, ++index) {
            set_channel_name(*pos, index);
        }
    }

    std::size_t slot_names::lookup(std::string const& name) const {
        auto const pos = named_slots_.find(name);
        return (pos != named_slots_.end())
            ? pos->second
            : std::numeric_limits<std::size_t>::max()
            ;
    }

}
