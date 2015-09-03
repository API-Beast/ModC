//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Environment.h"

namespace ModC
{
	void Environment::registerFunction(const std::string& name, NativeFunction fn, std::vector< int > parameters, int returnValue)
	{
		FunctionDeclaration& inserted = (Functions.Functions.insert({name, FunctionDeclaration()}))->second;
		inserted.Name = name;
		inserted.FnPtr = fn;
		inserted.Parameters = parameters;
		inserted.ReturnType = returnValue;
	}
	
	void Environment::registerMethod(int type, const std::string& name, NativeFunction fn, std::vector< int > parameters, int returnValue)
	{
		FunctionDeclaration& inserted = (Methods[type].Functions.insert({name, FunctionDeclaration()}))->second;
		inserted.Name = name;
		inserted.FnPtr = fn;
		inserted.Parameters = parameters;
		inserted.ReturnType = returnValue;
	}
	
	FunctionDeclaration* Environment::resolveFunctionCall(const std::string& name, const std::vector< int >& arguments, int returnType)
	{
		return Functions.resolve(name, arguments, returnType);
	}

	FunctionDeclaration* Environment::resolveMethodCall(int type, const std::string& name, const std::vector< int >& arguments, int returnType)
	{
		return Methods[type].resolve(name, arguments, returnType);
	}

	FunctionDeclaration* FunctionList::resolve(const std::string& name, const std::vector< int >& arguments, int returnType)
	{
		auto rate = [&](const FunctionDeclaration& behaviour)
		{
			if(behaviour.Parameters.size() != arguments.size()) return -1;
			if(returnType > -1) if(behaviour.ReturnType != returnType) return -1;
			int score = 1;
			for(int i = 0; i < arguments.size(); ++i)
			{
				if(behaviour.Parameters[i] == arguments[i])
					score += 100;
				else if(behaviour.Parameters[i] == -1)
					score += 70;
				// TODO: Conversation matrix? [int to float, etc]
				else
					return -1;
			}
			return score;
		};
		
		FunctionDeclaration* fn = nullptr;
		int fnScore = 0;
		auto range = Functions.equal_range(name);
		for(auto it = range.first; it != range.second; ++it)
		{
			FunctionDeclaration& beh = it->second;
			int points = rate(beh);
			if(points > fnScore)
			{
				fnScore = points;
				fn = &beh;
			}
		}
		
		return fn;
	}

}