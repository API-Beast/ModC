//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "ScriptEngine.h"
#include "Steps/1_Tokenize.h"
#include "Steps/2_Parse.h"
#include "Steps/3_Translate.h"

namespace ModC
{
	using namespace AST;

	void ModC::ScriptEngine::execute(const std::string& content)
	{
		ExpressionList  ast;
		InstructionList instructions;
		
		ModC::Tokenize(ast, content, Config);
		ModC::Parse(ast, Config);
		instructions = ModC::Translate(ast, Config);
		
		VirtualMachine vm;
		vm.Config      = &Config;
		vm.Environment = &Globals;
		vm.execute(instructions);
	};
}