#ifndef UTIL_TYPE_TYPETRAITS_H
#define UTIL_TYPE_TYPETRAITS_H

#include <type_traits>

// the code for are_same comes from this(https://stackoverflow.com/a/20047561) StackOverflow answer
template<class U, class...>
struct are_same : std::true_type
{};

template<class U, class V, class... UU>
struct are_same<U, V, UU...>
    : std::integral_constant<bool, std::is_same<U,V>{} && are_same<U, UU...>{}>
{};

// the code for and_ comes from this(https://stackoverflow.com/a/29603857) StackOverflow answer
template<typename... Conds>
struct and_ : std::true_type
{};

template<typename Cond, typename... Conds>
struct and_<Cond, Conds...>
    : std::conditional<Cond::value, and_<Conds...>, std::false_type>::type
{};

template<typename... UU>
using are_arithmetic = and_<std::is_arithmetic<UU>...>;

#endif