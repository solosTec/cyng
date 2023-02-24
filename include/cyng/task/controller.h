/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_TASK_CONTROLLER_H
#define CYNG_TASK_CONTROLLER_H

#include <cyng/task/registry.h>
#include <cyng/task/scheduler.h>
#include <cyng/task/task.hpp>

#include <boost/core/demangle.hpp>
#include <typeinfo>

namespace cyng {

    std::string cleanup_task_name(std::string const &);

    /**
     * Create channels/tasks with unique IDs.
     */
    class controller : public scheduler {
      public:
        controller();
        controller(std::size_t num_threads);

        registry &get_registry();

        /**
         * Stop all running tasks and the IO context
         */
        void shutdown();

        /** @brief create a task
         *
         * without additional parameters
         * un-named
         */
        template <typename T, typename... Args> [[nodiscard]] auto create_channel(Args &&...args) -> std::pair<channel_ptr, T *> {
            return create_channel_annotated<traits::no_ref, T>(std::forward<Args>(args)...);
        }

        /** @brief create a task
         *
         * with additional weak reference
         * un-named
         */
        template <typename T, typename... Args>
        [[nodiscard]] auto create_channel_with_ref(Args &&...args) -> std::pair<channel_ptr, T *> {
            return create_channel_annotated<traits::weak_ref, T>(std::forward<Args>(args)...);
        }

        /** @brief create a task
         *
         * without additional parameters
         * named
         */
        template <typename T, typename... Args>
        [[nodiscard]] auto create_named_channel(std::string name, Args &&...args) -> std::pair<channel_ptr, T *> {
            return create_named_channel_annotated<traits::no_ref, T>(name, std::forward<Args>(args)...);
        }

        /** @brief create a task
         *
         * with additional weak reference
         * named
         */
        template <typename T, typename... Args>
        [[nodiscard]] auto create_named_channel_with_ref(std::string name, Args &&...args) -> std::pair<channel_ptr, T *> {
            return create_named_channel_annotated<traits::weak_ref, T>(name, std::forward<Args>(args)...);
        }

      protected:
        /**
         * Create a task, insert this task into the registry and returns
         * the communication channel.
         */
        template <typename Traits, typename T, typename... Args>
        auto create_named_channel_annotated(std::string name, Args &&...args) -> std::pair<channel_ptr, T *> {
            //
            //	get new task id.
            //	Starts obviously with 1
            //
            auto const id = ++id_;

            //
            //	create a channel with an auto deleter
            //
            channel_ptr spc(new channel(get_ctx(), name), auto_remove(get_registry(), id));

            //	task pointer, task id
            task<T> *tsk = create_task<Traits, T>(spc, id, std::forward<Args>(args)...);
            BOOST_ASSERT(tsk != nullptr);

            //  get a pointer to the implementation
            T *impl = tsk->get_impl();
            BOOST_ASSERT(impl != nullptr);

            //
            //	update channel state
            //
            if (spc->open(tsk)) {

                //
                //	insert into registry (async)
                //
                get_registry().insert(spc);
            } else {
                BOOST_ASSERT_MSG(false, "open channel failed");
            }

            //
            //	make sure registry is updated
            //
            return {spc, impl};
        }

        /**
         * Create a task, insert this task into the registry and returns
         * the communication channel.
         */
        template <typename Traits, typename T, typename... Args>
        auto create_channel_annotated(Args &&...args) -> std::pair<channel_ptr, T *> {
            //
            //	get task name
            //
            using TASK = std::decay_t<T>;
            auto const name = boost::core::demangle(typeid(TASK).name());

            return create_named_channel_annotated<Traits, T>(cleanup_task_name(name), std::forward<Args>(args)...);
        }

        /**
         * task factory
         *
         * @tparam Traits controls behaviour of task constructor
         * @tparam T task implementation type
         * @tparam Args argument list for constructor of T
         * @return pointer to task pointer and the task id as std::pair<>
         */
        template <typename Traits, typename T, typename... Args>
        auto create_task(channel_ptr scp, std::size_t id, Args &&...args) -> task<T> * {

            //
            //	get new task id and
            //	create new task
            //
            return new task<T>(Traits{}, scp, &registry_, id, std::forward<Args>(args)...);
        }

      private:
        registry registry_;
        std::atomic<std::size_t> id_;
    };
} // namespace cyng

#endif
