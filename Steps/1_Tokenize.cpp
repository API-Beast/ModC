//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "1_Tokenize.h"
#include "../Configuration.h"

#include <Springbok/Parsing.hpp>
#include <cassert>
#include <iostream>

using namespace ModC;
using namespace ModC::AST;

void ModC::Tokenize(ExpressionList& tokens, const std::string& content, Configuration& config)
{
	StringParser p(content);
	p.StripWhitespace = false;
	
	const auto& operCharSet  = config.CharacterSets.Operator;
	const auto& opers        = config.Operators;
	const auto& identCharSet = config.CharacterSets.Identifier;
	
	// ------------------------------------------------------------------
	// ### Tokenizer
	// ------------------------------------------------------------------
	while(!p.atEnd())
	{
		Expression curExpression;
		Token& curToken = curExpression.Tok;
		Codepoint c = p.peek();
		auto isNumber = [](Codepoint c){return (c >= '0' && c <= '9');};
		if(isNumber(c))
		{
			curToken.Type  = Token::Number;
			curToken.Content = p.advanceWhile(isNumber);
			tokens.push_back(curExpression);
		}
		else if(operCharSet.count(c))
		{
			int pos = p.Position;
			std::string oper = p.advanceWhile([&](Codepoint c){  return operCharSet.count(c); });
			const std::vector<std::string>* operatorLists[] = 
			    {&opers.Prefix.Names, &opers.Postfix.Names,
			     &opers.PriorityBinary.Names, &opers.Binary.Names, &opers.Assignment.Names};
			bool found = false;
			
			for(auto* operators : operatorLists)
			for(const std::string& op : *operators)
			{
				if(found) break;
				if(oper.find(op) == 0)
				{
					p.Position = pos + op.size(); // op is already UTF8 so we can just skip in bytes.
					curToken.Type  = Token::Operator;
					curToken.Content = op;
					tokens.push_back(curExpression);
					found = true;
					//p.next();
					break;
				}
			}
			if(found) continue;
			
			p.Position = pos;
			p.next();
		}
		else if(c == '"')
		{
			curToken.Type  = Token::String;
			p.next();
			curToken.Content = p.advanceTo(U'"');
			tokens.push_back(curExpression);
		}
		else if(c == ';' || c == ',' || c == '.' || c == ':')
		{
			curToken.Type  = Token::Reserved;
			curToken.Content = UTF8::Encode(c);
			p.next();
			tokens.push_back(curExpression);
		}
		else if(c == '{' || c == '(' || c == '[')
		{
			curToken.Type  = Token::ParenOpen;
			curToken.Content = UTF8::Encode(c);
			p.next();
			tokens.push_back(curExpression);
		}
		else if(c == '}' || c == ')' || c == ']')
		{
			curToken.Type  = Token::ParenOpen;
			curToken.Content = UTF8::Encode(c);
			p.next();
			tokens.push_back(curExpression);
		}
		else if(identCharSet.count(c))
		{
			curToken.Content = p.advanceWhile([&](Codepoint c){ return identCharSet.count(c); });
			
			if(config.Keywords.count(curToken.Content))
				curToken.Type  = Token::Keyword;
			else
				curToken.Type  = Token::Identifier;
			
			tokens.push_back(curExpression);
		}
		else if(UCS::IsWhitespace(c) || UCS::IsInvisible(c))
		{
			p.advanceWhile([](Codepoint c){ return (UCS::IsWhitespace(c) || UCS::IsInvisible(c)); });
			// Whitespace; Ignore
		}
		else
		{
			assert(false);
			p.next();
		}
	}
}