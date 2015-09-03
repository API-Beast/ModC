//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "Type2Number.h"
#include <cassert>
#include <string>

namespace ModC
{

	struct ScriptValue
	{
		enum EFlags
		{
			NoFlags = 0,
			OwnedMemory = 1,
			VariableRef = 2
		};
		
		int   Type    = -1;
		int   Flags   = NoFlags;
		void* Pointer = nullptr;
		
		ScriptValue(){};
		ScriptValue(int i);
		ScriptValue(const std::string& val);
		
		ScriptValue(const ScriptValue& other){ *this = other; };
		ScriptValue(ScriptValue&& other)     { *this = other; };
		
		ScriptValue& operator=(const ScriptValue& other);
		ScriptValue& operator=(ScriptValue&& other);
		
		~ScriptValue();
		
		template<typename T>     bool is()   { return Type == Type2Number<T>(); };
		template<typename T> const T& as()   { if(Type == Type2Number<T>()) return *(T*)Pointer; else assert(false);};
		template<typename T>       T& asRef(){ if(Type == Type2Number<T>()) return *(T*)Pointer; else assert(false);};
	};

}