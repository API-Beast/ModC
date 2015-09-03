//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "../Internals/Instruction.h"
#include "../Internals/SyntaxTree.h"

namespace ModC
{
	struct Configuration;
	
	InstructionList Translate(const AST::ExpressionList& syntaxTree, const ModC::Configuration& config);
	void GenerateInstructions(InstructionList& target, const AST::Expression& syntaxTree, const ModC::Configuration& config);
}