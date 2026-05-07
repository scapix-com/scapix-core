/*
	scapix/core/meta/all_of.h

	Copyright (c) 2019-2024 Boris Rasin (boris@scapix.com)
*/

#ifndef SCAPIX_CORE_META_ALL_OF_H
#define SCAPIX_CORE_META_ALL_OF_H

namespace scapix::meta {

template <typename L, template <typename> class P>
struct all_of;

template <template <typename...> class L, typename ...E, template <typename> class P>
struct all_of<L<E...>, P>
{
	static constexpr bool value = (P<E>::value && ...);
};

template <typename L, template <typename> class P>
concept all_of_v = all_of<L, P>::value;

} // namespace scapix::meta

#endif // SCAPIX_CORE_META_ALL_OF_H
