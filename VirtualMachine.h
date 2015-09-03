//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include <string>
#include <map>

#include "Internals/Instruction.h"
#include "Internals/ScriptValue.h"

namespace ModC
{
	struct Configuration;
	struct Environment;
	
	struct VirtualMachine
	{
		ModC::Configuration* Config;
		ModC::Environment*   Environment;
		
		struct StackValue{ ScriptValue* Value = nullptr; int Flags = -1; };
		StackValue Stack[64];
		int        StackPosition = 0;
		
		struct CallStackValue{ int ReturnAddress = -1; int StackPosition = -1; };
		CallStackValue CallStack[64];
		int            CallStackPosition = 0;
		
		std::map<std::string, ScriptValue> Variables;
		
		void execute(const InstructionList& toExec);
		void callFunction(const std::string& name);
		
		void push(const ScriptValue& arg);
		void pushRef(ScriptValue* arg);
		ScriptValue  pop();
		ScriptValue* popRef();
		const ScriptValue& peek(int i = 0);
		void clearStack();
	};

}
