//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "CppBind.h"
#include "Internals/Type2Number.h"

namespace ModC
{
	template<typename A, typename B = A>
	void BindBasicMathOperators(Environment &e)
	{
		int a = Type2Number<A>();
		int b = Type2Number<B>();
		std::vector<int> p = {b};

		e.registerMethod(a, "_add",      &NativeWrapper::Add     <A, B>,  p, Type2Number<decltype(A() + B())>());
		e.registerMethod(a, "_subtract", &NativeWrapper::Subtract<A, B>,  p, Type2Number<decltype(A() - B())>());
		e.registerMethod(a, "_multiply", &NativeWrapper::Multiply<A, B>,  p, Type2Number<decltype(A() * B())>());
		e.registerMethod(a, "_divide",   &NativeWrapper::Divide  <A, B>,  p, Type2Number<decltype(A() / B())>());
	}

	template<typename A, typename B = A>
	void BindComparisonOperators(Environment &e)
	{
		int a = Type2Number<A>();
		int b = Type2Number<B>();
		std::vector<int> p = {b};

		e.registerMethod(a, "_lessThan",    &NativeWrapper::LessThan     <A, B>, p);
		e.registerMethod(a, "_moreThan",    &NativeWrapper::MoreThan     <A, B>, p);
		e.registerMethod(a, "_lessOrEqual", &NativeWrapper::LessOrEqualTo<A, B>, p);
		e.registerMethod(a, "_moreOrEqual", &NativeWrapper::MoreOrEqualTo<A, B>, p);
		e.registerMethod(a, "_equal",       &NativeWrapper::EqualTo      <A, B>, p);
		e.registerMethod(a, "_unequal",     &NativeWrapper::UnequalTo    <A, B>, p);
	}

	template<typename A>
	void BindPrefixOperators(Environment& e)
	{
		int a = Type2Number<A>();

		e.registerMethod(a, "_negate",   &NativeWrapper::Negate  <A>, {}, Type2Number<decltype(-A())>());
		e.registerMethod(a, "_identity", &NativeWrapper::Identity<A>, {}, Type2Number<decltype(+A())>());
		e.registerMethod(a, "_not",      &NativeWrapper::Not     <A>, {}, Type2Number<decltype(!A())>());
	}
}