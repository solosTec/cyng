/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_TASK_HPP
#define CYNG_TASK_HPP

#include <cyng/meta.hpp>
#include <cyng/obj/function_cast.hpp>
#include <cyng/obj/intrinsics/eod.h>
#include <cyng/obj/object.h>
#include <cyng/obj/util.hpp>
#include <cyng/task/registry.h>
#include <cyng/task/task_interface.h>

#include <boost/core/ignore_unused.hpp>

namespace cyng {

    namespace {
        template <std::size_t N> struct invoke_helper {
            template <typename Tpl> static cyng::tuple_t call(std::size_t idx, tuple_t const &msg, Tpl &tpl) {
                using F = typename std::tuple_element<N, Tpl>::type;
#ifdef __DEBUG_TEST
                std::cout << "call: " << idx << ", " << N << std::endl;
#endif
                if (idx == N) {
                    using R = typename F::result_type;
                    if constexpr (std::is_same<R, void>::value) {
                        function_call<F>(std::get<N>(tpl), msg);
                        return cyng::make_tuple();
                    } else {
                        return cyng::make_tuple(function_call<F>(std::get<N>(tpl), msg));
                    }
                }
                return invoke_helper<N - 1>::call(idx, msg, tpl);
            }
        };

        template <> struct invoke_helper<0> {
            template <typename Tpl> static cyng::tuple_t call(std::size_t idx, tuple_t const &msg, Tpl &tpl) {

                using F = typename std::tuple_element<0, Tpl>::type;
                using R = typename F::result_type;
#ifdef __DEBUG_TEST
                std::cout << "call: " << idx << ", " << 0 << std::endl;
#else
                boost::ignore_unused(idx);
#endif
                if constexpr (std::is_same<R, void>::value) {
                    function_call<F>(std::get<0>(tpl), msg);
                    return cyng::make_tuple();
                } else {
                    return cyng::make_tuple(function_call<F>(std::get<0>(tpl), msg));
                }
            }
        };

        template <std::size_t N> struct invoke_f_helper {
            template <typename Tpl>
            static void call(std::size_t idx, tuple_t const &msg, Tpl &tpl, std::function<void(tuple_t &&msg)> f) {
                using F = typename std::tuple_element<N, Tpl>::type;
#ifdef __DEBUG_TEST
                std::cout << "invoke_r: " << idx << ", " << N << std::endl;
#endif
                if (idx == N) {
                    using R = typename F::result_type;
                    if constexpr (std::is_same<R, void>::value) {
                        function_call<F>(std::get<N>(tpl), msg);
                        f(cyng::make_tuple()); //	void
                    } else {
                        f(cyng::make_tuple(function_call<F>(std::get<N>(tpl), msg)));
                    }
                    return;
                }
                invoke_f_helper<N - 1>::call(idx, msg, tpl, f);
            }
        };

        template <> struct invoke_f_helper<0> {
            template <typename Tpl>
            static void call(std::size_t idx, tuple_t const &msg, Tpl &tpl, std::function<void(tuple_t &&msg)> f) {

                using F = typename std::tuple_element<0, Tpl>::type;
                using R = typename F::result_type;
#ifdef __DEBUG_TEST
                std::cout << "invoke_r: " << idx << ", " << 0 << std::endl;
#else
                boost::ignore_unused(idx);
#endif
                if constexpr (std::is_same<R, void>::value) {
                    function_call<F>(std::get<0>(tpl), msg);
                    f(cyng::make_tuple()); //	void
                } else {
                    f(cyng::make_tuple(function_call<F>(std::get<0>(tpl), msg)));
                }
            }
        };

    } // namespace

    namespace traits {
        /**
         * Initialize implementation class with a weak reference
         * of the channel object. Enables to start a timer.
         */
        struct weak_ref {};

        /**
         * No additional parameters are passed to the implementation class
         */
        struct no_ref {};
    } // namespace traits

    template <typename T> class task : public task_interface {
        friend class channel;
        friend class registry;
        using seq_t = std::make_index_sequence<std::tuple_size_v<typename T::signatures_t>>;
        constexpr static std::size_t signature_count_ = std::tuple_size<typename T::signatures_t>::value - 1u;

        using stop_fn = std::function<void(eod)>;
        constexpr static std::size_t stop_idx_ = tmp::index<stop_fn, typename T::signatures_t>::value;

      public:
        template <typename... Args>
        task(traits::no_ref, channel_ptr scp, registry *reg, std::size_t id, Args &&...args)
            : registry_(reg)
            , id_(id)
            , impl_(std::forward<Args>(args)...) {
            boost::ignore_unused(scp);
        }

        template <typename... Args>
        task(traits::weak_ref, channel_ptr scp, registry *reg, std::size_t id, Args &&...args)
            : registry_(reg)
            , id_(id)
            //	task not complete at this stage
            , impl_(scp, std::forward<Args>(args)...) {}

        virtual ~task() {}

        virtual std::size_t get_id() const noexcept override { return id_; }

        virtual std::size_t get_signature_count() const noexcept { return signature_count_ + 1u; }

        T *get_impl() { return &impl_; }

      protected:
        virtual cyng::tuple_t dispatch(std::size_t slot, tuple_t const &msg, channel_ptr) override {
            BOOST_ASSERT(slot <= signature_count_);
            return (slot > signature_count_) ? cyng::make_tuple() : invoke_helper<signature_count_>::call(slot, msg, impl_.sigs_);
        }
        virtual void stop(std::shared_ptr<channel> sp) override {
            //
            //	call the last entry
            //
            dispatch(stop_idx_, cyng::make_tuple(eod{}), sp);

            //
            //	delete this task
            //
            try {
                delete this;
            } catch (...) {
            }

            //
            //	after this: undefined behaviour
            //
        }

        virtual void next(std::size_t slot, std::function<void(tuple_t &&msg)> f, tuple_t const &msg) override {
            BOOST_ASSERT(slot <= signature_count_);
            if (slot > signature_count_)
                return;
            //	apply result of function as argument to f(result)
            invoke_f_helper<signature_count_>::call(slot, msg, impl_.sigs_, f);
        }

      private:
        registry *registry_;
        std::size_t const id_;
        T impl_; //!<	implementation class
    };
} // namespace cyng

#endif
