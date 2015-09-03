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
	
	enum ResolveFlags
	{
		Aggressive    = 0,
		Shy           = 1,
		Collection    = 2,
		AvoidKeywords = 4
	};
	
	void Parse(AST::ExpressionList& tokens, ModC::Configuration& config);
	
	void ResolveSugar    (AST::Iterator it, const std::string& keyword, const std::vector< std::string >& syntax);
	void ResolveOperators(AST::Iterator it, const std::vector< std::string >& oper, int argsL, int argsR, int flags = Aggressive);
	void ResolveTrinary  (AST::Iterator it, const std::string& start, const std::string& end, int argsL, int argsR, int flags = Aggressive);
	void ResolveBinding  (AST::Iterator it, const std::string& a, const std::string& b);
};