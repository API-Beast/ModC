//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include <map>
#include <string>

#include "Internals/ScriptValue.h"
#include "Internals/FunctionDeclaration.h"

namespace ModC
{
	using VariableState = std::map<std::string, ScriptValue>;
	
	struct FunctionList
	{
		std::multimap<std::string, FunctionDeclaration> Functions;
		FunctionDeclaration* resolve(const std::string& name, const std::vector< int >& arguments, int returnType = -1);
	};
	
	struct Environment
	{
		std::map<int, FunctionList> Methods;
		
		FunctionList  Functions;
		
		VariableState  DefaultValues;
		VariableState* LocalState = nullptr;
		
		void registerFunction(const std::string& name, NativeFunction fn, std::vector<int> parameters, int returnValue = -1);
		void registerMethod(int type, const std::string& name, NativeFunction fn, std::vector<int> parameters, int returnValue = -1);
		
    FunctionDeclaration* resolveFunctionCall(const std::string& name, const std::vector< int >& arguments, int returnType = -1);
    FunctionDeclaration* resolveMethodCall(int type, const std::string& name, const std::vector< int >& arguments, int returnType = -1);
	};
	
}
