/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#pragma once

#include <tuple>

namespace cyng {
    namespace tmp { //	template meta programming

        /**
         *	helper to convert data types
         * @see http://pdimov.com/cpp2/simple_cxx11_metaprogramming.html
         *
         * @tparam List list type (tuple)
         * @tparam Mod modifier
         * @tparam Args parameter
         */
        template <template <typename...> class List, template <typename> class Mod, typename... Args> struct transform {
            //
            //	Mod<Args> transforms the parameter list
            //
            using type = List<typename Mod<Args>::type...>;
        };

        template <template <typename...> class List, template <typename> class Mod, typename... Args>
        using transform_t = typename transform<List, Mod, Args...>::type;

        /**
         * Helper template to build the final template for the VM.
         *
         * Example:
         * @code
                tuple_cat_t<
                        std::tuple<int,float>,
                        std::tuple<int>
                > test{ 2, 3.0f, 4 };
         * @endcode
         */
        template <typename... Args> using tuple_cat_t = decltype(std::tuple_cat(std::declval<Args>()...));

        /**
         * Calculate the index of a specified type in the tag tuple:
         *
         * Example:
         * @code
         * using foo_t = std::tuple<int, char*, int, long>;
         * std::cout << "index<int, foo_t> = " << index<int, foo_t>::value << std::endl;
         * @endcode
         */
        template <class T, class Tuple> struct index {
            static_assert(!std::is_same_v<Tuple, std::tuple<>>, "Could not find T in given Tuple");
        };

        template <class T, class... Types> struct index<T, std::tuple<T, Types...>> {
            static constexpr std::size_t value = 0;
        };

        template <class T, class U, class... Types> struct index<T, std::tuple<U, Types...>> {
            static constexpr std::size_t value = 1 + index<T, std::tuple<Types...>>::value;
        };

        //
        //	https://stackoverflow.com/questions/35190992/how-do-i-create-a-type-list-to-expand-into-a-tuple
        //

        /**
         * Example:
         * @code
         * template<class... T>
                struct ComponentList {};

                template<class List>
                struct ComponentManager {
                        rebind<List, std::tuple> components;
                };

                int main() {
                        using List = ComponentList<int, char, long>;
                        ComponentManager<List> manager;
                        std::cout << std::get<0>(manager.components) << '\n';
                }
         * @endcode
         */
        template <class A, template <class...> class B> struct rebind_;

        template <template <class...> class A, class... T, template <class...> class B> struct rebind_<A<T...>, B> {
            using type = B<T...>;
        };

        template <class A, template <class...> class B> using rebind = typename rebind_<A, B>::type;

    } // namespace tmp
} // namespace cyng
