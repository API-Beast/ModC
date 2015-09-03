//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include <Springbok/Parsing/UTF8.h>

#include <vector>
#include <set>
#include <map>

namespace ModC
{

	struct Type
	{
		std::string Name;
		int MemorySize  =  0;
		int Inheritance = -1;
	};
	
	struct OperatorList
	{
		std::vector<std::string> Names;
		std::vector<std::string> FnCalls;
		void add(std::string str, std::string func);
	};
	
	struct Configuration
	{
		struct
		{
			OperatorList Prefix;
			OperatorList Postfix;
			
			OperatorList PriorityBinary;// Dot before Line: a*b, a/b, a%b
			OperatorList Binary;        // a+b, a-b, bitwise operators, comparison operators
			OperatorList Assignment;    // Assignment operators
		} Operators;
		
		struct 
		{
			std::set<Codepoint> Identifier;
			std::set<Codepoint> Operator;
		} CharacterSets;
		
		std::set<std::string> Keywords;
		std::map<int, Type>   NativeTypes;
		
		void extractOperatorCharSet();

		template<typename T>
		int registerScriptType(const std::string& name);
	};

}

#include "Configuration_Templates.hpp"