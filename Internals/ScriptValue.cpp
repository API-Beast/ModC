//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "ScriptValue.h"

namespace ModC
{
	
ScriptValue::ScriptValue(int i)
{
	Pointer = new int(i);
	Type    = Type2Number<int>();
	Flags   = OwnedMemory;
}

ScriptValue::ScriptValue(const std::string& val)
{
	Pointer = new std::string(val);
	Type    = Type2Number<std::string>();
	Flags   = OwnedMemory;
}

ScriptValue& ScriptValue::operator=(ScriptValue&& other)
{
	Pointer = other.Pointer;
	Flags   = other.Flags;
	Type    = other.Type;
	other.Flags = NoFlags;
	return *this;
}

ScriptValue& ScriptValue::operator=(const ScriptValue& other)
{
	Flags   = other.Flags;
	Type    = other.Type;
	if(Flags & OwnedMemory)
	{
		if(Type == Type2Number<int>())
			Pointer = new int(*(int*)other.Pointer);
		else if(Type == Type2Number<std::string>())
			Pointer = new std::string(*(std::string*)other.Pointer);
		else
			assert(false);
	}
	else
		Pointer = other.Pointer;
	return *this;
}

ScriptValue::~ScriptValue()
{
	if(Flags & OwnedMemory)
	{
		if(Type == Type2Number<int>())
			delete (int*)Pointer;
		else if(Type == Type2Number<int>())
			delete (std::string*)Pointer;
	}
}

}