#include <cyng/task/channel.h>
#include <cyng/task/task.hpp>
#include <cyng/obj/object.h>

#ifdef _DEBUG_TASK
#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>
#include <cyng/io/ostream.h>
#endif

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
        if (is_open()) {

            //
            //  the context should be active
            //
            BOOST_ASSERT(!dispatcher_.context().stopped());

            //
            //	thread safe access to task
            //
            message m(this->shared_from_this(), slot, std::move(msg));
            auto sp = this->shared_from_this(); //  extend life time

            boost::asio::post(dispatcher_, [this, sp, m]()->void {
                if (is_open())  task_->dispatch(m.slot_, m.msg_);
             });
        }
    }

    void channel::dispatch(std::string slot, tuple_t&& msg) {
#ifdef _DEBUG_TASK
        //  search for a bug in "db.req.update"
        if (boost::algorithm::equals(slot, "db.req.update")) {
            std::cout
                << std::endl
                << std::endl
                << slot
                << ": "
                << msg
                << std::endl
                << std::endl
                ;
        }
        
#endif
        dispatch(lookup(slot), std::move(msg));
    }

    bool channel::is_open() const noexcept
    {
        return task_.operator bool();
    }

    bool channel::stop()
    {
        //
        //  mark channel as closed
        //
        if (is_open()) {

            //
            //  cancel timer
            //
            timer_.cancel();

            auto r = shutdown(boost::asio::use_future);
            return r.get();
        }
        return false;
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

    std::size_t slot_names::lookup(std::string const& name) const {
        auto const pos = named_slots_.find(name);
        return (pos != named_slots_.end())
            ? pos->second
            : std::numeric_limits<std::size_t>::max()
            ;
    }

}
