/*
	scapix/core/fixed_string.h

	Copyright (c) 2019-2026 Boris Rasin (boris@scapix.com)
*/

#ifndef SCAPIX_CORE_FIXED_STRING_H
#define SCAPIX_CORE_FIXED_STRING_H

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <string_view>
#include <type_traits>

namespace scapix {

template <typename T>
concept character =
	std::same_as<T, char> ||
	std::same_as<T, wchar_t> ||
	std::same_as<T, char8_t> ||
	std::same_as<T, char16_t> ||
	std::same_as<T, char32_t>;

template <typename T, typename CharT = std::remove_cv_t<std::remove_extent_t<std::remove_reference_t<T>>>>
concept string_literal =
	std::is_lvalue_reference_v<T> &&
	std::is_array_v<std::remove_reference_t<T>> &&
	std::is_const_v<std::remove_extent_t<std::remove_reference_t<T>>> &&
	character<CharT> &&
	std::same_as<CharT, std::remove_cv_t<std::remove_extent_t<std::remove_reference_t<T>>>>; 

template <string_literal L>
using literal_char = std::remove_cv_t<std::remove_extent_t<std::remove_reference_t<L>>>;

template <string_literal L>
inline constexpr std::size_t literal_size = std::extent_v<std::remove_reference_t<L>> - 1;

template <character CharT, std::size_t N>
struct fixed_string
{
	CharT content[N + 1]{};

	constexpr fixed_string() noexcept = default;

	constexpr fixed_string(const CharT(&literal)[N + 1]) noexcept
	{
		for (std::size_t i = 0; i <= N; ++i)
			content[i] = literal[i];
	}

	constexpr fixed_string replaced(CharT from, CharT to) const noexcept
	{
		fixed_string result = *this;

		for (std::size_t i = 0; i < N; ++i)
			if (result.content[i] == from)
				result.content[i] = to;

		return result;
	}

	constexpr std::size_t size() const noexcept { return N; }
	constexpr const CharT* data() const noexcept { return content; }
	constexpr const CharT* begin() const noexcept { return content; }
	constexpr const CharT* end() const noexcept { return content + N; }
	constexpr CharT operator[](std::size_t i) const noexcept { return content[i]; }
	constexpr operator std::basic_string_view<CharT>() const noexcept { return { content, N }; }
	constexpr operator const CharT*() const noexcept { return content; }
};

template <string_literal L>
fixed_string(L&&) -> fixed_string<literal_char<L>, literal_size<L>>;

template <character CharT, std::size_t N1, std::size_t N2>
constexpr fixed_string<CharT, N1 + N2> operator + (const fixed_string<CharT, N1>& lhs, const fixed_string<CharT, N2>& rhs) noexcept
{
	fixed_string<CharT, N1 + N2> result;

	for (std::size_t i = 0; i < N1; ++i)
		result.content[i] = lhs.content[i];

	for (std::size_t i = 0; i < N2; ++i)
		result.content[N1 + i] = rhs.content[i];

	return result;
}

template <character CharT, std::size_t N>
constexpr auto operator + (string_literal<CharT> auto&& lhs, const fixed_string<CharT, N>& rhs) noexcept
{
	return fixed_string<CharT, literal_size<decltype(lhs)>>(lhs) + rhs;
}

template <character CharT, std::size_t N>
constexpr auto operator + (const fixed_string<CharT, N>& lhs, string_literal<CharT> auto&& rhs) noexcept
{
	return lhs + fixed_string<CharT, literal_size<decltype(rhs)>>(rhs);
}

template <character CharT, std::size_t N1, std::size_t N2>
constexpr bool operator == (const fixed_string<CharT, N1>& lhs, const fixed_string<CharT, N2>& rhs) noexcept
{
	if constexpr (N1 != N2)
		return false;
	else
		return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

// C++20 also tries the reversed argument order.

template <character CharT, std::size_t N>
constexpr bool operator == (const fixed_string<CharT, N>& lhs, string_literal<CharT> auto&& rhs) noexcept
{
	return lhs == fixed_string<CharT, literal_size<decltype(rhs)>>(rhs);
}

template <typename T>
concept fixed_size_string =
	!std::is_array_v<std::remove_cvref_t<T>> &&
	requires (const std::remove_cvref_t<T>&t)
{
	{ std::size(t) } -> std::convertible_to<std::size_t>;
	requires character<std::remove_cvref_t<decltype(*std::data(t))>>;

	typename std::bool_constant < (std::size(std::remove_cvref_t<T>{}), true) > ;
	typename std::bool_constant < (std::data(std::remove_cvref_t<T>{}), true) > ;
};

// Any type whose character count is known at compile time from the type alone.

template <typename T>
concept compile_time_string = string_literal<T> || fixed_size_string<T>;

template <compile_time_string T>
consteval std::size_t string_size() noexcept
{
	if constexpr (string_literal<T>)
		return literal_size<T>;
	else
		return std::size(std::remove_cvref_t<T>{});
}

} // namespace scapix

#endif // SCAPIX_CORE_FIXED_STRING_H
