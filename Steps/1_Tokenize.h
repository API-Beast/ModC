//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "../Internals/SyntaxTree.h"
#include <string>

namespace ModC
{
	struct Configuration;
	
	void Tokenize(AST::ExpressionList& tokens, const std::string& content, ModC::Configuration& config);
};