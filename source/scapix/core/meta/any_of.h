/*
	scapix/core/meta/any_of.h

	Copyright (c) 2019-2026 Boris Rasin (boris@scapix.com)
*/

#ifndef SCAPIX_CORE_META_ANY_OF_H
#define SCAPIX_CORE_META_ANY_OF_H

namespace scapix::meta {

template <typename L, template <typename> class P>
struct any_of;

template <template <typename...> class L, typename ...E, template <typename> class P>
struct any_of<L<E...>, P>
{
	static constexpr bool value = (P<E>::value || ...);
};

template <typename L, template <typename> class P>
concept any_of_v = any_of<L, P>::value;

} // namespace scapix::meta

#endif // SCAPIX_CORE_META_ANY_OF_H
