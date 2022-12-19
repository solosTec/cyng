/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_CONTAINER_FACTORY_HPP
#define CYNG_OBJ_CONTAINER_FACTORY_HPP

#include <cyng/obj/factory.hpp>
#include <cyng/obj/util.hpp>

namespace cyng {

    /**
     * Create an object from each parameter element and add
     * it to a list of objects (tuple_t)
     */
    template <typename... Args> [[nodiscard]] object tuple_factory(Args &&...args) {
        return make_object(cyng::make_tuple(std::forward<Args>(args)...));
    }

    template <typename T> [[nodiscard]] object vector_factory(std::vector<T> const &data) { return make_object(make_vector(data)); }

    template <typename... Args> [[nodiscard]] object deque_factory(Args &&...args) {
        return make_object(make_deque(std::forward<Args>(args)...));
    }

    /**
     * Create an object of type attribute
     */
    template <typename... Args> [[nodiscard]] object pair_factory(std::size_t idx, Args &&...args) {
        return make_object(make_attr(idx, std::forward<Args>(args)...));
    }

    /**
     * Create an object of type parameter
     */
    template <typename... Args> [[nodiscard]] object pair_factory(std::string const &name, Args &&...args) {
        BOOST_ASSERT_MSG(!name.empty(), "parameter without a name");
        return make_object(make_param(name, std::forward<Args>(args)...));
    }

    namespace detail {
        /**
         * generic implementation class for map factories
         */
        template <typename T> class map_factory {
            //
            //  get key type
            //
            using K = typename T::key_type;

          public:
            map_factory()
                : map_() {}

            template <typename U>
            map_factory(K key, U &&v)
                : map_() {
                map_.emplace(key, make_object(v));
            }

            template <typename U> map_factory operator()(K key, U &&v) {
                map_.emplace(key, make_object(v));
                return *this;
            }

            operator T () const { return map_; }
            T const &to_map() const { return map_; }
            T get_map() const { return map_; }

            /**
             * convinience conversion
             */
            object operator()() const { return make_object(map_); }

          private:
            T map_;
        };
    } // namespace detail

    /**
     * example
     * @code
     attr_map_t m = attr_map_factory(1, "1")(2, "2");
     * @endcode
     */
    using attr_map_factory = detail::map_factory<attr_map_t>;

    /**
     * example
     * @code
     param_map_t m = param_map_factory("1", 1)("2", 2);
     * @endcode
     */
    using param_map_factory = detail::map_factory<param_map_t>;

    /**
     * example
     * @code
     prop_map_t m = prop_map_factory(make_obis(1,2,3,4,5,6), 1)(make_obis(6,5,4,3,2,1), 2);
     * @endcode
     */
    using prop_map_factory = detail::map_factory<prop_map_t>;

} // namespace cyng

#endif //	CYNG_OBJ_FACTORY_HPP
