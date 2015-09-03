//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "2_Parse.h"
#include "../Configuration.h"

using namespace ModC;
using namespace ModC::AST;

void ModC::Parse(ModC::AST::ExpressionList& tokens, ModC::Configuration& config)
{
	ResolveTrinary(&tokens, "{", "}", 0, 0, ResolveFlags::Collection);
	ResolveTrinary(&tokens, "(", ")", 0, 0, ResolveFlags::Collection | ResolveFlags::Shy | ResolveFlags::AvoidKeywords);
	
	ResolveTrinary  (&tokens, "(", ")", 1, ResolveFlags::Aggressive); // Function call
	ResolveOperators(&tokens, {"."}, 1, 1, ResolveFlags::Aggressive);
	ResolveTrinary  (&tokens, "[", "]", 1, ResolveFlags::Aggressive); // Index operator
	
	ResolveSugar(&tokens, "switch", {"(", ")"});
	
	auto& op = config.Operators;
	
	ResolveOperators(&tokens, op.Postfix.Names, 1, 0, ResolveFlags::Shy);
	ResolveOperators(&tokens, op. Prefix.Names, 0, 1, ResolveFlags::Shy);
	
	ResolveOperators(&tokens, op.PriorityBinary.Names, 1, 1, ResolveFlags::Aggressive);
	ResolveOperators(&tokens, op.        Binary.Names, 1, 1, ResolveFlags::Aggressive);
	
	ResolveOperators(&tokens, op.Postfix.Names, 1, 0, ResolveFlags::Aggressive);
	ResolveOperators(&tokens, op. Prefix.Names, 0, 1, ResolveFlags::Aggressive);
	
	ResolveOperators(&tokens, op.Assignment.Names, 1, 1, ResolveFlags::Aggressive);
	
	ResolveOperators(&tokens, {","}, 1, 1);
	
	ResolveSugar(&tokens, "return", {});
	
	ResolveOperators(&tokens, {";"}, 1, 0);
	ResolveOperators(&tokens, {";"}, 0, 0);
	
	ResolveSugar(&tokens, "if",     {"(", ")"});
	ResolveSugar(&tokens, "else",   {});
	ResolveSugar(&tokens, "while",  {"(", ")"});
	ResolveSugar(&tokens, "for",    {"(", "in", ")"});
	
	ResolveSugar(&tokens, "case",   {"(", ")"});
	
	ResolveBinding(&tokens, "if", "else");
}

void ModC::ResolveBinding(Iterator it, const std::string& a, const std::string& b)
{
	while(!it.atEnd())
	{
		if(it.value(0).Tok == a && it.value(1).Tok == b)
			it.consumeR(1);
		it.advance();
	}
}

void ModC::ResolveSugar(Iterator it, const std::string& keyword, const std::vector< std::string >& syntax)
{
	while(!it.atEnd())
	{
		auto& exp = it.value();
		if(!exp.resolved())
		{
			bool isMatch = true;
			if(it.value(0).Tok.Content != keyword)                     isMatch = false;
			if(it.remainingItems() < (1 + (int(syntax.size()) - 1)*2)) isMatch = false; // Stupid STL, unsigned int is for losers
			int i = 0;
			while(isMatch)
			{
				if(i >= syntax.size()) break;
				if(it.value(1 + i*2).Tok.Content != syntax[i]) isMatch = false;
				i++;
			}
			if(isMatch)
			{
				exp.Type = Expression::FlowStatement;
				bool first = true;
				for(const std::string& val : syntax)
				{
					if(!first) it.consumeR(1);
					it.eraseR();
					first = false;
				}
				it.consumeR(1);
			}
		}
		else if(!exp.fullyResolved())
		{
			ResolveSugar(&(exp.Right), keyword, syntax);
			ResolveSugar(&(exp.Left) , keyword, syntax);
		};
		it.advance();
	}
}

void ModC::ResolveTrinary(Iterator it, const std::string& start, const std::string& end, int argsL, int argsR, int flags)
{
	bool resolvedMatch = false;
	bool shy           = flags & Shy;
	bool collection    = flags & Collection;
	bool avoidKeywords = flags & AvoidKeywords;
	while(!it.atEnd())
	{
		auto& exp = it.value();
		if(!exp.resolved())
		{
			if(exp.Tok == start)
			{
				     if(!it.canConsumeL(argsL));         // We only can check for the left side as long as we don't know the end
				else if(shy           && it.canConsumeL(argsL+1));
				else if(avoidKeywords && it. isKeywordL(argsL+1));
				else
				{
					Iterator endIt = it;
					endIt.advance();
					int level = 1;
						
					while(!endIt.atEnd())
					{
						if(!endIt.value().resolved())
						{
							if(endIt.value().Tok == end)
								level--;
							if(endIt.value().Tok == start)
								level++;
							if(level == 0)
								break;
						}
						endIt.advance();
					}
					if(level == 0)
					{
						     if(!endIt.canConsumeR(argsR));
						else if(shy           && endIt.canConsumeR(argsR+1));
						else if(avoidKeywords && endIt. isKeywordL(argsR+1));
						else
						{
							exp.Tok.Content = start + end;
							exp.Type = collection ? Expression::Collection : Expression::Operator;
							it.merge(endIt);
							it.eraseR(1);
							it.consumeL(argsL);
							it.consumeR(argsR);
							resolvedMatch = true;
						}
					}
				}
			}
		}
		else if(!exp.fullyResolved())
		{
			ResolveTrinary(&(exp.Right), start, end, argsL, argsR, flags);
			ResolveTrinary(&(exp.Left ), start, end, argsL, argsR, flags);
		};
		it.advance();
	}
	if(resolvedMatch)
	{
		it.Position = it.Start;
		ResolveTrinary(it, start, end, argsL, argsR, flags);
	}
}

void ModC::ResolveOperators(Iterator it, const std::vector< std::string >& oper, int argsL, int argsR, int flags)
{
	bool resolvedMatch = false;
	bool shy = flags & Shy;
	while(!it.atEnd())
	{
		Expression& exp = it.value();
		if(!exp.resolved())
		{
			bool matches = false;
			for(const std::string& val : oper)
				if(exp.Tok == val)
					matches = true;
				
			if(matches)
			{
				     if(!it.canConsumeL(argsL));
				else if(!it.canConsumeR(argsR));
				else if(shy && it.canConsumeL(argsL+1)); // More than this operator can chew, don't consume anything
				else if(shy && it.canConsumeR(argsR+1)); // Shy = Only consume if it is a exact match.
				else
				{
					it.consumeL(argsL);
					it.consumeR(argsR);
					it.value().Type = Expression::Operator;
					resolvedMatch = true;
				}
			}
		}
		else if(!exp.fullyResolved())
		{			
			ResolveOperators(&(it.value().Right), oper, argsL, argsR, flags);
			ResolveOperators(&(it.value().Left ), oper, argsL, argsR, flags);
		};
		it.advance();
	}
	if(resolvedMatch)
	{
		it.Position = it.Start;
		ResolveOperators(it, oper, argsL, argsR, flags);
	}
}