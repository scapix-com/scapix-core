/*
	scapix/core/meta/none_of.h

	Copyright (c) 2019-2024 Boris Rasin (boris@scapix.com)
*/

#ifndef SCAPIX_CORE_META_NONE_OF_H
#define SCAPIX_CORE_META_NONE_OF_H

namespace scapix::meta {

template <typename L, template <typename> class P>
struct none_of;

template <template <typename...> class L, typename ...E, template <typename> class P>
struct none_of<L<E...>, P>
{
	static constexpr bool value = !(P<E>::value || ...);
};

template <typename L, template <typename> class P>
concept none_of_v = none_of<L, P>::value;

} // namespace scapix::meta

#endif // SCAPIX_CORE_META_NONE_OF_H
