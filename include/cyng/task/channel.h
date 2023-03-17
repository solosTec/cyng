/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_TASK_CHANNEL_H
#define CYNG_TASK_CHANNEL_H

#include <atomic>
#include <boost/asio.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>

#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/util.hpp>
#include <cyng/task/task_interface.h>

namespace cyng {

    class channel;
    using channel_ptr = std::shared_ptr<channel>;
    using channel_weak = std::weak_ptr<channel>;

    /**
     * Base class to manage named slots
     */
    class slot_names {
      public:
        slot_names();

        /**
         * Specify the name of a channel
         */
        bool set_channel_name(std::string, std::size_t);

        /**
         * New names will be added at the end of channel name list
         */
        void set_channel_names(std::initializer_list<std::string> il);

        /**
         * @name name of the function
         * @return std::numeric_limits<std::size_t>::max() if slot was not found
         */
        std::pair<std::size_t, bool> lookup(std::string const &name) const;

      private:
        /**
         * optionally slots can be named
         */
        std::unordered_map<std::string, std::size_t> named_slots_;
    };

    //
    //	forward declarations
    //
    class task_interface;
    class controller;
    class channel;
    template <typename Token> void exec(channel &, Token &&);
    template <typename Token> bool exec(channel_ptr cp, Token &&token);
    boost::asio::io_context::strand &expose_dispatcher(channel &);

    class channel : public std::enable_shared_from_this<channel>, public slot_names {
        template <typename T> friend class task;
        friend class controller;
        template <typename Token> friend void exec(channel &, Token &&);
        template <typename Token> friend bool exec(channel_ptr, Token &&);
        friend boost::asio::io_context::strand &expose_dispatcher(channel &);

      public:
        //
        //	timer defines clock and timepoint type
        //	std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>
        //
        using timer_t = boost::asio::steady_timer;
        using clock_t = timer_t::clock_type;
        using time_point_t = clock_t::time_point;

        class result {
          public:
            result(channel &, std::size_t slot, tuple_t &&msg);
            result(result &) = delete; // deleted
            bool is_ready() const;
            std::pair<cyng::tuple_t, bool> get_result() const;

          private:
            std::atomic<bool> ready_;
            tuple_t result_;
        };
        friend result;

        /**
         * error callback, with channel name and undefined slot name.
         */
        using cb_err_t = std::function<void(std::string, std::string)>;

      public:
        channel(boost::asio::io_context &io, std::string name);

        /**
         * @return true, if channel is open
         */
        bool is_open() const noexcept;

        /**
         * @return true, if channel is open and slot is in the valid range
         */
        bool is_open(std::size_t slot) const noexcept;

        /**
         * Send a message to the specified slot of the task object.
         * Works asynchronously.
         *
         * @param slot slot ID
         * @param msg object list that will be casted to the required function signature.
         */
        void dispatch(std::size_t slot, tuple_t &&msg);
        void dispatch(std::size_t slot);

        template <typename... Args> void dispatch(std::size_t slot, Args &&...args) {
            dispatch(slot, cyng::make_tuple(std::forward<Args>(args)...));
        }

        /**
         * Takes the slot from the named slot table
         */
        void dispatch(std::string slot, cb_err_t cb, tuple_t &&msg);
        void dispatch(std::string slot, cb_err_t cb);

        template <typename... Args> void dispatch(std::string slot, cb_err_t cb, Args &&...args) {
            dispatch(slot, cb, cyng::make_tuple(std::forward<Args>(args)...));
        }

        /**
         * Close this communication channel
         * @return true if channel was closed, false if channel was already closed
         */
        bool stop();

        /*
         * cancel timer
         *
         * @return true if timer was active
         */
        bool cancel_timer();

        /**
         * Channel names must not be unique.
         *
         * @return the channel name
         */
        std::string const &get_name() const noexcept;

        /**
         * forwarded to task interface
         */
        std::size_t get_id() const noexcept;

        /**
         * time-delayed execution
         */
        template <typename R, typename P> void suspend(std::chrono::duration<R, P> d, std::size_t slot, tuple_t &&msg) {

            if (!is_open(slot))
                return;

            auto sp = this->shared_from_this(); //  extend life time

            //	formerly: expires_from_now()
            //	Since boost 1.66.0
            timer_.expires_after(d);

            timer_.async_wait(boost::asio::bind_executor(dispatcher_, [this, sp, slot, msg](boost::system::error_code const &ec) {
                if (ec != boost::asio::error::operation_aborted && is_open(slot)) {
                    task_->dispatch(slot, msg, sp);
                }
            }));
        }

        template <typename R, typename P>
        void suspend(std::chrono::duration<R, P> d, std::string name, cb_err_t cb, tuple_t &&msg) {
            auto const [slot, ok] = lookup(name);
            if (ok) {
                suspend(d, slot, std::move(msg));
            } else {
                if (cb) {
                    cb(name_, name);
                }
            }
        }

        template <typename R, typename P, typename... Args>
        void suspend(std::chrono::duration<R, P> d, std::string name, cb_err_t cb, Args &&...args) {
            auto const [slot, ok] = lookup(name);
            if (ok) {
                suspend(d, slot, cyng::make_tuple(std::forward<Args>(args)...));
            } else {
                if (cb) {
                    cb(name_, name);
                }
            }
        }

        bool suspend(time_point_t tp, std::size_t slot, tuple_t &&msg);

        bool suspend(time_point_t tp, std::string slot, cb_err_t cb, tuple_t &&msg);

        template <typename... Args> bool suspend(time_point_t tp, std::string name, cb_err_t cb, Args &&...args) {
            auto const [slot, ok] = lookup(name);
            if (ok) {
                return suspend(tp, slot, cyng::make_tuple(std::forward<Args>(args)...));
            }
            if (cb) {
                cb(name_, name);
            }
            return false;
        }

        /**
         * Calls function(slot_producer) and pass result as parameter to function(slot_consumer).
         *
         * @param slot_producer slot id of the producer function
         * @param slot_consumer slot id of the consumer function
         * @param msg parameters for for producer function
         */
        void next(std::size_t slot_producer, std::size_t slot_consumer, tuple_t &&msg);
        void next(std::string slot_producer, std::string slot_consumer, cb_err_t cb, tuple_t &&msg);

        /**
         * Calls function(slot_producer) and pass result as parameter to function(slot_consumer).
         *
         * @param slot_producer slot id of the producer function
         * @param slot_producer consumer channel object with the consumer function
         * @param slot_consumer slot id of the consumer function
         * @param msg parameters for for producer function
         */
        void next(std::size_t slot_producer, channel_ptr consumer, std::size_t slot_consumer, tuple_t &&msg);

        template <typename... Args> result defer(std::string name, cb_err_t cb, Args &&...args) {
            auto const [slot, ok] = lookup(name);
            if (ok) {
                return result(*this, slot, cyng::make_tuple(std::forward<Args>(args)...));
            }
            cb(name_, name);
            return result(*this, slot, {});
        }

      private:
        void dispatch_r(std::size_t slot, tuple_t &&msg, std::function<void(cyng::tuple_t)>);

        /**
         * apply result of function(slot) as argument to f(result)
         */
        void next(std::size_t slot, std::function<void(tuple_t &&msg)> f, tuple_t &&msg);
        void next(std::string slot, cb_err_t cb, std::function<void(tuple_t &&msg)> f, tuple_t &&msg);

        /**
         * open channel by providing a task interface
         *
         */
        template <typename T> bool open(T *tp) {
            //	mark channel as open
            if (!is_open()) {
                task_.reset(tp);
                return true;
            }
            return false;
        }

      private:
        boost::asio::io_context::strand dispatcher_;

        /**
         * Internal timer for suspend mode
         */
        boost::asio::steady_timer timer_;

        /**
         * task_ ptr represents the "open" state of the channel.
         * A channel is "open" when the task pointer is not null.
         */
        std::unique_ptr<task_interface> task_;

        /**
         * Each channel has a non-unique name
         */
        std::string const name_;
    };

    /**
     * comparison for equality
     */
    inline bool operator==(channel const &lhs, channel const &rhs) { return lhs.get_id() == rhs.get_id(); }
    inline bool operator!=(channel const &lhs, channel const &rhs) { return !(lhs == rhs); }

    /**
     * Use the same strand as the channel
     */
    template <typename Token> void exec(channel &ch, Token &&token) {
        boost::asio::post(ch.dispatcher_, std::forward<Token>(token));
    }
    template <typename Token> bool exec(channel_ptr cp, Token &&token) {
        if (cp) {
            boost::asio::post(cp->dispatcher_, std::forward<Token>(token));
            return true;
        }
        return false;
    }

} // namespace cyng

#include <functional>

namespace std {

    template <> class hash<cyng::channel_ptr> {
      public:
        size_t operator()(cyng::channel_ptr) const noexcept;
    };
    template <> class hash<cyng::channel_weak> {
      public:
        size_t operator()(cyng::channel_weak) const noexcept;
    };
} // namespace std
#endif
