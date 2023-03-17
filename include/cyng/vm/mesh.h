/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_VM_MESH_H
#define CYNG_VM_MESH_H

#include <cyng/task/controller.h>
#include <cyng/vm/proxy.h>

#include <type_traits>

#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace cyng {

    template <typename... Fns> class vm;

    /**
     * ToDo: add constraint that F must be a std::function<()> template
     *	requires std::is_function_v(F)
     */
    template <typename F> struct description {
        using function_t = F;
        function_t f_;
        std::string const name_;

        description(std::string name, function_t f)
            : f_(f)
            , name_(name) {}
    };

    template <typename R, typename... Args>
    auto make_description(std::string name, std::function<R(Args...)> f) -> description<std::function<R(Args...)>> {
        using function_t = typename std::function<R(Args...)>;
        using type = description<std::function<R(Args...)>>;
        return description<function_t>(name, std::forward<function_t>(f));
    }

    template <typename D> constexpr auto get_function(D d) -> typename D::function_t { return d.f_; }

    template <typename D> constexpr auto get_name(D d) -> std::string { return d.name_; }

    /**
     * Managing VM instances
     */
    class mesh {
        /**
         * Grant access to controller to enable message postings.
         */
        template <typename... Fns> friend class vm;

      public:
        explicit mesh(controller &) noexcept;

        template <typename... Fns> vm_proxy create_proxy(channel::cb_err_t cb, Fns &&...fns) {
            return create_vm(cb, std::forward<Fns>(fns)...);
        }

        template <typename... Fns> vm_proxy create_proxy(boost::uuids::uuid parent, channel::cb_err_t cb, Fns &&...fns) {
            return create_vm(parent, cb, std::forward<Fns>(fns)...);
        }

        template <typename... D> vm_proxy make_proxy(channel::cb_err_t cb, D &&...ds) {

            vm_proxy proxy = create_vm(cb, get_function<D>(ds)...);

#ifdef _DEBUG
            //((std::cout << get_name<D>(ds)), ...);
#endif

            //
            //	set channel names
            //
            proxy.set_channel_names({get_name<D>(ds)...});
            return proxy;
        }

        template <typename... D> vm_proxy make_proxy(boost::uuids::uuid parent, channel::cb_err_t cb, D &&...ds) {

            vm_proxy proxy = create_vm(parent, cb, get_function<D>(ds)...);

            //
            //	set channel names
            //
            proxy.set_channel_names({get_name<D>(ds)...});
            return proxy;
        }

        /**
         * Search for a VM with the specified tag
         */
        void lookup(boost::uuids::uuid tag, std::function<void(std::vector<channel_ptr>)> cb);

        /**
         * get access to task manager
         */
        controller &get_ctl();

      private:
        template <typename... Fns> channel_ptr create_vm(channel::cb_err_t cb, Fns &&...fns) {
            return create_vm(boost::uuids::nil_uuid(), cb, std::forward<Fns>(fns)...);
        }

        template <typename... Fns> channel_ptr create_vm(boost::uuids::uuid parent, channel::cb_err_t cb, Fns &&...fns) {

            //
            //	create an uuid
            //
            auto const tag = uuid_rgn_();

            //
            //	create task channel
            //
            using vm_t = vm<typename std::decay<Fns>::type...>; //	no references
            return ctl_
                .create_named_channel_with_ref<vm_t>(boost::uuids::to_string(tag), *this, cb, parent, std::forward<Fns>(fns)...)
                .first;
        }

      private:
        controller &ctl_;
        boost::uuids::random_generator uuid_rgn_;
    };

    /**
     * Adapter templates to build vm functions.
     * Reference parameters not supported.
     *
     * Example:
     * @code
     * auto vm = fabric.create_proxy(vm_adaptor<session, std::uint64_t, std::string>(&s, &session::foo));
     * @endcode
     */
    template <typename T, typename R, typename... Args> std::function<R(Args...)> vm_adaptor(T *p, R (T::*ptr)(Args...)) {
        return [p, ptr](Args... args) { return ((*p).*ptr)(std::forward<Args>(args)...); };
    }

    template <typename T, typename R, typename... Args> std::function<R(Args...)> vm_adaptor(T *p, R (T::*ptr)(Args...) const) {
        return [p, ptr](Args... args) { return ((*p).*ptr)(std::forward<Args>(args)...); };
    }

} // namespace cyng
#endif
