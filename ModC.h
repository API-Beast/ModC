//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "ScriptEngine.h"
#include "Environment.h"

namespace ModC
{
	ScriptEngine ModScript();

	void AssignAny(ModC::VirtualMachine* vm);

	template<typename A> void BindPrefixOperators(Environment& e);
	template<typename A> void BindAssignmentOperators(Environment& e);

	template<typename A, typename B = A> void BindBasicMathOperators (Environment& e);
	template<typename A, typename B = A> void BindComparisonOperators(Environment& e);
};

#include "ModC_Templates.hpp"
