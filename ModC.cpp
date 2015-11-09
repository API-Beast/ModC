//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "ModC.h"
#include "CppBind.h"

namespace ModC
{

ScriptEngine ModScript()
{
	ScriptEngine s;
	auto& c = s.Config;
	auto& g = s.Globals;
	
	// Prefix operators : --a, ++a, -a, +a, !a
	{
		auto& ops = c.Operators.Prefix;
		ops.add( "--", "_decrement");
		ops.add( "++", "_increment");
		ops.add(  "-", "_negate"   );
		ops.add(  "+", "_identity" );
		ops.add(  "!", "_not");
	}
	
	// TODO: Postfix operators
	// -----------------------
	
	// Priority binary operators: a*b, a/b, a%b
	{
		auto& ops = c.Operators.PriorityBinary;
		ops.add("*", "_multiply");
		ops.add("/", "_divide");
		ops.add("%", "_modulo");
	}
	
	// Binary operators: a+b, a-b, etc.
	{
		auto& ops = c.Operators.Binary;
		ops.add("+", "_add");
		ops.add("-", "_subtract");
		
		ops.add(">=", "_moreOrEqual");
		ops.add("<=", "_lessOrEqual");
		ops.add( "<", "_lessThan");
		ops.add( ">", "_moreThan");
		
		ops.add("==", "_equal");
		ops.add("!=", "_unequal");
		
		ops.add("&&", "_and");
		ops.add("||", "_or");
	}
	
	// Assignment operators: a=b, a-=b, etc.
	{
		auto& ops = c.Operators.Assignment;
		ops.add( "=", "_assign");
		ops.add("*=", "_multiplyAssign");
		ops.add("/=", "_divideAssign");
		ops.add("+=", "_addAssign");
		ops.add("-=", "_subtractAssign");
	}
	
	for(Codepoint j = 'A'; j <= 'Z'; ++j)
		c.CharacterSets.Identifier.insert(j);
	for(Codepoint j = 'a'; j <= 'z'; ++j)
		c.CharacterSets.Identifier.insert(j);
	for(Codepoint j = '0'; j <= '9'; ++j)
		c.CharacterSets.Identifier.insert(j);
	
	c.extractOperatorCharSet();
	
	c.registerScriptType<int>("int");
	c.registerScriptType<std::string>("string");

	// TODO: Where does this belong?
	// We need it to be able to assign to unassigned variables.
	c.NativeTypes[-2].Name = "nullvar";
	g.registerMethod(-2, "_assign", ModC::AssignAny, {-1});

	BindPrefixOperators    <int>(g);
	BindBasicMathOperators <int>(g);
	BindComparisonOperators<int>(g);
	//BindAssignmentOperators<int>(g);
	
	return s;
}

void AssignAny(ModC::VirtualMachine* vm)
{
	ModC::ScriptValue* ref = vm->popRef();
	*ref = *(vm->popRef());
	vm->pushRef(ref);
}

}
