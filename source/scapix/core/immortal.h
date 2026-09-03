/*
	scapix/core/immortal.h

	Copyright (c) 2019-2024 Boris Rasin (boris@scapix.com)
*/

#ifndef SCAPIX_CORE_IMMORTAL_H
#define SCAPIX_CORE_IMMORTAL_H

#include <cstddef>
#include <memory>
#include <utility>

namespace scapix {

// Leak object on destruction.
// Used to declare global objects which cannot be destroyed.
// Conceptually, T should satisfy boost::has_trivial_destructor_after_move<T>.

template <typename T>
class immortal : public T
{
public:

	using T::T;
	using T::operator=;

	immortal(const immortal&) = delete;
	immortal& operator=(const immortal&) = delete;
	immortal(immortal&&) = delete;
	immortal& operator=(immortal&&) = delete;

	~immortal()
	{
		alignas(T) std::byte temp[sizeof(T)];
		std::construct_at(reinterpret_cast<T*>(temp), std::move(*this));
	}

};

} // namespace scapix

#endif // SCAPIX_CORE_IMMORTAL_H
