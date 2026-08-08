/*
	scapix/core/separator.h

	Copyright (c) 2019-2024 Boris Rasin (boris@scapix.com)
*/

#ifndef SCAPIX_CORE_SEPARATOR_H
#define SCAPIX_CORE_SEPARATOR_H

#include <utility>
#include <type_traits>

namespace scapix {

/*

Evaluates to one value the first time and another value all other times:

separator comma("", ", ");

for (auto x : list)
	os << comma << x;

*/

template <typename T>
class separator
{
public:

	constexpr separator(T first, T rest, bool first_time = true) :
		first_time(first_time),
		first(std::move(first)),
		rest(std::move(rest))
	{
	}

	constexpr void reset(bool first_time = true)
	{
		this->first_time = first_time;
	}

	constexpr explicit operator bool() const
	{
		return first_time;
	}

	constexpr operator const T&()
	{
		if (first_time)
		{
			first_time = false;
			return first;
		}

		return rest;
	}

private:

	bool first_time = true;
	T first;
	T rest;

};

} // namespace scapix

#endif // SCAPIX_CORE_SEPARATOR_H
