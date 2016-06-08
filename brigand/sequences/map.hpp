/*==================================================================================================
  Copyright (c) 2015 Edouard Alligand and Joel Falcou

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
=================================================================================================**/
#pragma once
#include <type_traits>
#include <brigand/types/type.hpp>
#include <brigand/sequences/pair.hpp>
#include <brigand/types/no_such_type.hpp>

namespace brigand
{

namespace lazy
{
    template <typename M, typename K>
    struct lookup
    : decltype(M::at(type_<K>{}))
    {};
}

    template <typename M, typename K>
    using lookup = typename lazy::lookup<M,K>::type;

namespace detail
{
    template <class... T>
    struct map_impl;

    template <>
    struct map_impl<>
    {
        template <typename U>
        static type_<no_such_type_> at(U);

        template <class K>
        static map_impl<K> insert(type_<K>);
    };

    template <class... Ts>
    struct map_impl
    {
    private:
        struct Pack : pair<typename Ts::first_type, Ts>... {};

        template<class K, class P>
        static type_<typename P::second_type> at_impl(pair<K,P>*);

        template <class K, typename U>
        static map_impl<Ts...> insert_impl(U);

        template <class K>
        static map_impl<Ts..., K> insert_impl(type_<no_such_type_>);

    public:
        template<class K>
        static decltype(at_impl<K>(static_cast<Pack*>(nullptr))) at(type_<K>);

        template<class K>
        static type_<no_such_type_> at(K);

        template <class P>
        static decltype(insert_impl<P>(at(type_<typename P::first_type>{}))) insert(type_<P>);
    };

    // if you have a "class already a base" error message, it means you have defined a map with the same key present more
    // than once, which is an error
    template<class... Ts>
    struct make_map : type_<typename Ts::first_type>... {
      using type = map_impl<Ts...>;
    };
}
    template<class... Ts>
    using map = typename detail::make_map<Ts...>::type;
}
