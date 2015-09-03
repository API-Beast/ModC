//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "SyntaxTree.h"

namespace ModC
{
namespace AST
{
	
	Expression::Expression()
	{
		Right.reserve(8);
		Left.reserve(2);
	}

	bool Expression::resolved() const
	{
		return Type != SingleValue;
	}

	bool Expression::fullyResolved() const
	{
		if(!resolved()) return false;
		bool resolved = true;
		for(const Expression& exp : Right)
			resolved = resolved && exp.fullyResolved();
		for(const Expression& exp : Left)
			resolved = resolved && exp.fullyResolved();
		return resolved;
	}

	bool Iterator::canConsumeL(int arguments)
	{
		if((Position - Start) < arguments) return false;
		
		for(int i = 1; i <= arguments; ++i)
			if(value(-i).isSymbol())
				return false;
		return true;
	}

	bool Iterator::canConsumeR(int arguments)
	{
		if((End - Position) < arguments) return false;
		
		for(int i = 1; i <= arguments; ++i)
			if(value(i).isSymbol())
				return false;
		return true;
	}

	bool Iterator::isKeywordL(int arguments)
	{
		if((Position - Start) < arguments) return false;
		return value(-arguments).isKeyword();
	}

	bool Iterator::isKeywordR(int arguments)
	{
		if((End - Position) < arguments) return false;
		return value(arguments).isKeyword();
	}

	void Iterator::consumeL(int arguments)
	{
		auto& op = value();
		for(int i = 1; i <= arguments; ++i)
			op.Left.push_back(value(-i));
		for(int i = 1; i <= arguments; ++i)
		{
			Target->erase(Target->begin()+(Position-1));
			Position--;
		}
		End -= arguments;
	}

	void Iterator::consumeR(int arguments)
	{
		auto& op = value();
		for(int i = 1; i <= arguments; ++i)
			op.Right.push_back(value(+i));
		
		eraseR(arguments);
	}

	void Iterator::eraseR(int arguments)
	{
		for(int i = 1; i <= arguments; ++i)
			Target->erase(Target->begin()+(Position+1));
		End -= arguments;
	}

	void Iterator::merge(const Iterator& end)
	{
		int args = end.Position - Position - 1;
		if(args > 0)
			consumeR(args);
	}
	
	Iterator::Iterator(ExpressionList* target)
	{
		Target = target;
		Start = 0;
		End = Target->size()+1;
		Position = 0;
	}
	
}
}