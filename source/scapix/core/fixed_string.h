/*
	scapix/core/fixed_string.h

	Copyright (c) 2019-2026 Boris Rasin (boris@scapix.com)
*/

#ifndef SCAPIX_CORE_FIXED_STRING_H
#define SCAPIX_CORE_FIXED_STRING_H

#include <concepts>
#include <cstddef>
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

template <typename T, typename CharT = std::remove_const_t<std::remove_extent_t<std::remove_reference_t<T>>>>
concept string_literal = character<CharT> && std::same_as<T, const CharT(&)[std::extent_v<std::remove_reference_t<T>>]>;

template <character CharT, std::size_t N>
struct fixed_string
{
	CharT content[N + 1]{};

	constexpr fixed_string() noexcept = default;

	constexpr fixed_string(const CharT(&literal)[N + 1]) noexcept
	{
		for (std::size_t i = 0; i < N; ++i)
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

	template <std::size_t M>
	constexpr fixed_string<CharT, N + M> operator + (const fixed_string<CharT, M>& rhs) const noexcept
	{
		fixed_string<CharT, N + M> result;

		for (std::size_t i = 0; i < N + M; ++i)
			result.content[i] = i < N ? content[i] : rhs.content[i - N];

		return result;
	}

	constexpr auto operator + (string_literal<CharT> auto&& rhs) const noexcept { return *this + scapix::fixed_string(rhs); }
	friend constexpr auto operator + (string_literal<CharT> auto&& lhs, const fixed_string& rhs) noexcept { return scapix::fixed_string(lhs) + rhs; }

	// C++20 also tries the reversed argument order.

	template <std::size_t M>
	constexpr bool operator == (const fixed_string<CharT, M>& rhs) const noexcept { return std::basic_string_view<CharT>(*this) == rhs; }
	constexpr bool operator == (string_literal<CharT> auto&& rhs) const noexcept { return *this == scapix::fixed_string(rhs); }

	constexpr std::size_t size() const noexcept { return N; }
	constexpr const CharT* data() const noexcept { return content; }
	constexpr const CharT* begin() const noexcept { return content; }
	constexpr const CharT* end() const noexcept { return content + N; }
	constexpr CharT operator[](std::size_t i) const noexcept { return content[i]; }
	constexpr operator std::basic_string_view<CharT>() const noexcept { return { content, N }; }
	constexpr operator const CharT*() const noexcept { return content; }
};

template <character CharT, std::size_t N>
fixed_string(const CharT(&)[N]) -> fixed_string<CharT, N - 1>;

template <typename T>
inline constexpr bool is_fixed_string = false;

template <typename CharT, std::size_t N>
inline constexpr bool is_fixed_string<fixed_string<CharT, N>> = true;

template <typename T>
concept compile_time_string = string_literal<T> || is_fixed_string<std::remove_cvref_t<T>>;

template <compile_time_string T>
consteval std::size_t string_size() noexcept
{
	if constexpr (string_literal<T>)
		return std::extent_v<std::remove_reference_t<T>> - 1;
	else
		return std::remove_cvref_t<T>{}.size();
}

} // namespace scapix

#endif // SCAPIX_CORE_FIXED_STRING_H
