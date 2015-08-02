
#include <type_traits>
#include <brigand/types/compose.hpp>
#include <brigand/functions/plus.hpp>
#include <brigand/types/integer.hpp>

static_assert(std::is_same<brigand::unary_compose<brigand::plus, brigand::int_<2>>::type<brigand::int_<1>>, brigand::int_<3>>::value, "invalid result");
static_assert(std::is_same<brigand::binary_compose<brigand::plus>::type<brigand::int_<2>, brigand::int_<1>>, brigand::int_<3>>::value, "invalid result");

template<class T, class...> struct always_first { using type = T; };

static_assert(std::is_same<brigand::compose<always_first, brigand::int_<2>>::type<brigand::int_<1>>::type, brigand::int_<2>>::value, "invalid result");
static_assert(std::is_same<brigand::compose<always_first>::type<brigand::int_<2>, brigand::int_<1>>::type, brigand::int_<2>>::value, "invalid result");
