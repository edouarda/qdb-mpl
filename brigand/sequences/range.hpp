/*==================================================================================================
  Copyright (c) 2015 Edouard Alligand and Joel Falcou

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
=================================================================================================**/
#pragma once

#include <type_traits>

#include <brigand/sequences/list.hpp>

namespace brigand
{
  namespace detail
  {
    template<class T, class, class, T>
    struct range_cat;

    template<class T, T Start, T Int>
    struct int_plus
    {
      using type = std::integral_constant<T, Start + Int>;
    };

    template<class T, class... Ts, T... Ints, T Start>
    struct range_cat<T, list<Ts...>, list<std::integral_constant<T, Ints>...>, Start>
    {
      using type = list<Ts..., typename int_plus<T, Start, Ints>::type...>;
    };

    template<class T, T Start, std::size_t N>
    struct range_impl
    : range_cat<
      T,
      typename range_impl<T, Start, N/2>::type,
      typename range_impl<T, Start, N - N/2>::type,
      N/2
    >
    {};

    template<class T, T Start>
    struct range_impl<T, Start, 1>
    {
      using type = list<std::integral_constant<T, Start>>;
    };

    template<class T, T Start>
    struct range_impl<T, Start, 0>
    {
      using type = list<>;
    };

    template<class T, T Start, class L, std::size_t N>
    struct reverse_range_adapt;

    template<class T, T Start, std::size_t... Ints, std::size_t N>
    struct reverse_range_adapt<T, Start, list<std::integral_constant<T, Ints>...>, N>
    {
      using type = list<typename int_plus<T, T(N - Ints), Start>::type...>;
    };

    template<class T, T Start, std::size_t N>
    struct reverse_range_impl
    : reverse_range_adapt<T, Start, typename range_impl<T, 0, N>::type, N>
    {};
  }

  template<class T, T Start, T Stop>
  using range = typename detail::range_impl<T, Start, Stop-Start>::type;

  template<class T, T Start, T Stop>
  using reverse_range = typename detail::reverse_range_impl<T, Stop, Start-Stop>::type;
}
