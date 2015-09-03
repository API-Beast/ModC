//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "ScriptValue.h"
#include "FunctionDeclaration.h"


namespace ModC
{

	enum InstructionType
	{
		CONVERT,
		FUNCALL, OBJCALL,   CALL,
		PUSH,    GETVAR,    GETSUB,
		JUMPTO,  JUMPIF,    JUMPIFN,
		SUBCALL, SUBENTER,  RETURN,
		EOL,     LABEL
	};

	struct Instruction
	{
		Instruction(){};
		Instruction(const std::string& name, int arguments);
		Instruction(FunctionDeclaration    func);
		
		Instruction(InstructionType t)                       : Type(t){};
		Instruction(InstructionType t, int l)                : Type(t), LabelID(l){};
		Instruction(InstructionType t, const ScriptValue& v) : Type(t), Value(v){};
		Instruction(InstructionType t, const std::string& n) : Type(t), Name(n){};
		Instruction(InstructionType t, const std::string& n, int a)     : Type(t), Name(n),     NumArguments(a){};
		Instruction(InstructionType t, NativeFunction b, int a) : Type(t), Function(b), NumArguments(a){};
		
		InstructionType Type;
		
		// DYNAMIC, REESOLVED, PUSHVAR, ACCESS
		std::string Name = "<undefined>";
		// DYNAMIC, RESOLVED
		int NumArguments = -1;
		// CONVERT
		int TargetType = -1;
		// PUSHVAL
		ScriptValue Value;
		// RESOLVED
		NativeFunction Function;
		// LABEL, JMP, JMPIF, JMPIFN
		int LabelID = -1;
	};
	
	using InstructionList = std::vector<Instruction>;
}