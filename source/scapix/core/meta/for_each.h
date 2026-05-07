/*
	scapix/core/meta/for_each.h

	Copyright (c) 2019-2024 Boris Rasin (boris@scapix.com)
*/

#ifndef SCAPIX_CORE_META_FOR_EACH_H
#define SCAPIX_CORE_META_FOR_EACH_H

#include <utility>

namespace scapix::meta {
namespace detail {

template <class L>
struct for_each_impl;

template <template <class...> class L, class ...T>
struct for_each_impl<L<T...>>
{
	template <class F>
	static constexpr void call(F&& f)
	{
		(std::forward<F>(f).template operator()<T>(), ...);
	}
};

template <template <auto...> class L, auto ...V>
struct for_each_impl<L<V...>>
{
	template <class F>
	static constexpr void call(F&& f)
	{
		(std::forward<F>(f).template operator()<V>(), ...);
	}
};

} // namespace detail

template <class L, class F>
constexpr void for_each(F&& f)
{
	detail::for_each_impl<L>::call(std::forward<F>(f));
}

} // namespace scapix::meta

#endif // SCAPIX_CORE_META_FOR_EACH_H
