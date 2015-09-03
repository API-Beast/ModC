//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "Configuration.h"
#include <Springbok/Parsing/UTF8.h>
#include <Springbok/Parsing/Unicode.h>

namespace ModC
{
	void OperatorList::add(std::string str, std::string func)
	{
		Names.push_back(str);
		FnCalls.push_back(func);
	}

	void Configuration::extractOperatorCharSet()
	{
		auto addChars = [&](const std::string& op)
		{
			int pos = 0;
			Codepoint cur = UTF8::DecodeAt(op, pos);
			while(pos < op.size())
			{
				if(!UCS::IsWhitespace(cur))
				if(!CharacterSets.Identifier.count(cur))
					CharacterSets.Operator.insert(cur);
				cur = UTF8::DecodeNext(op, &pos);
			}
		};
		
		for(const std::string& op : Operators.Prefix        .Names) addChars(op);
		for(const std::string& op : Operators.Postfix       .Names) addChars(op);
		for(const std::string& op : Operators.Assignment    .Names) addChars(op);
		for(const std::string& op : Operators.Binary        .Names) addChars(op);
		for(const std::string& op : Operators.PriorityBinary.Names) addChars(op);
	}

}