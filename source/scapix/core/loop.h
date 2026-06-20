/*
	scapix/core/loop.h

	Copyright (c) 2019-2024 Boris Rasin (boris@scapix.com)
*/

#ifndef SCAPIX_CORE_LOOP_H
#define SCAPIX_CORE_LOOP_H

#include <utility>

namespace scapix {

template <typename F>
class except_first_time
{
public:

	except_first_time(F&& func) : function(std::move(func)) {}
	void reset() { first = true; }

	void operator ()()
	{
		if (first)
			first = false;
		else
			function();
	}

private:

	bool first = true;
	F function;

};

template <typename F>
class only_first_time
{
public:

	only_first_time(F&& func) : function(std::move(func)) {}
	void reset() { first = true; }
	explicit operator bool() const { return first; }

	void operator ()()
	{
		if (first)
		{
			first = false;
			function();
		}
	}

private:

	bool first = true;
	F function;

};

template <typename F>
class optional_first_time
{
public:

	optional_first_time(bool first, F&& func) : first(!first), function(std::move(func)) {}

	void operator ()()
	{
		if (first)
			first = false;
		else
			function();
	}

private:

	bool first = true;
	F function;

};

template <typename F1, typename F2>
class different_first_time
{
public:

	different_first_time(F1&& func1, F2&& func2) : function1(std::move(func1)), function2(std::move(func2)) {}
	void reset() { first = true; }

	void operator ()()
	{
		if (first)
		{
			first = false;
			function1();
		}
		else
		{
			function2();
		}
	}

private:

	bool first = true;
	F1 function1;
	F2 function2;

};

} // namespace scapix

#endif // SCAPIX_CORE_LOOP_H
