//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "3_Translate.h"
#include "../Configuration.h"

#include <iostream>
#include <algorithm>

namespace ModC
{
	using namespace AST;

	InstructionList Translate(const ExpressionList& syntaxTree, const Configuration& config)
	{
		InstructionList retVal;

		for(const Expression& exp : syntaxTree)
			GenerateInstructions(retVal, exp, config);

		return retVal;
	}
	
	void GenerateInstructions(InstructionList& target, const Expression& exp, const Configuration& config)
	{
		static int LabelCounter = 0;
		using ExprCRef = const Expression&;
		
		if(exp.isValue())
		{
					 if(exp.Tok.Type == Token::Number)     target.emplace_back(PUSH, ScriptValue(std::stoi(exp.Tok.Content)));
			else if(exp.Tok.Type == Token::String)     target.emplace_back(PUSH, ScriptValue(exp.Tok.Content));
			else if(exp.Tok.Type == Token::Identifier) target.emplace_back(GETVAR, exp.Tok.Content);
			else std::cout << "Unimplemented constant: " << exp.Tok.Content << std::endl;
			return;
		}
		
		if(exp.Type == Expression::Collection)
		{
			for(ExprCRef subexp : exp.Right)
				GenerateInstructions(target, subexp, config);
			for(ExprCRef subexp : exp.Left)
				GenerateInstructions(target, subexp, config);
			return;
		}

		bool expandLeft  = true;
		bool expandRight = true;
		std::vector<Instruction> code;
		auto& opers = config.Operators;
		const OperatorList* operatorLists[] = {&opers.PriorityBinary, &opers.Binary, &opers.Assignment};
		
		if(exp.Tok.Content == "()")
		{
			code.emplace_back(FUNCALL, exp.Left[0].Tok.Content, exp.Right.size());
			expandLeft = false;
		}
		else if(exp.Tok == ";")
		{
			code.emplace_back(EOL);
		}
		else if(exp.Tok == ".")
		{
			// Member function call
			if(exp.Right[0].Tok.Content == "()")
			{
				// Manual expansion
				expandLeft  = false;
				expandRight = false;
				ExprCRef callexp = exp.Right[0];
				
				GenerateInstructions(target, exp.Left[0], config);
				for(ExprCRef subexp : callexp.Right)
					GenerateInstructions(target, subexp, config);
				target.emplace_back(OBJCALL, callexp.Left[0].Tok.Content, callexp.Right.size());
			}
			// Member variable access
			else
			{
				code.emplace_back(GETSUB, exp.Right[0].Tok.Content);
				expandRight = false;
			}
		}
		else if(exp.isBinOp())
		{
			bool found = false;
			for(auto* operators : operatorLists)
			{
				int pos = std::find(operators->Names.begin(), operators->Names.end(), exp.Tok.Content) - operators->Names.begin();
				if(pos == operators->Names.size()) continue;
				code.emplace_back(OBJCALL, operators->FnCalls[pos], 1);
				found = true;
				break;
			}
			if(!found) std::cout << "Unimplemented binop: " << exp.Tok.Content << std::endl;
		}
		else if(exp.isPreOp())
		{
			auto& prefixOps = opers.Prefix.Names;
			int pos = std::find(prefixOps.begin(), prefixOps.end(), exp.Tok.Content) - prefixOps.begin();
			if(pos != prefixOps.size())
				code.emplace_back(OBJCALL, opers.Prefix.FnCalls[pos], 0);
			else
				std::cout << "Unimplemented preop: " << exp.Tok.Content << std::endl;
		}
		else if(exp.isPostOp())
		{     
			auto& postfixOps = opers.Postfix.Names;
			int pos = std::find(postfixOps.begin(), postfixOps.end(), exp.Tok.Content) - postfixOps.begin();
			if(pos != postfixOps.size())
				code.emplace_back(OBJCALL, opers.Postfix.FnCalls[pos], 0);
			else
				std::cout << "Unimplemented postop: " << exp.Tok.Content << std::endl;
		}
		else if(exp.Type == Expression::FlowStatement)
		{
			// We will be doing this manually for correct placement
			expandLeft  = false;
			expandRight = false;
			
			if(exp.Tok == "if")
			{
				int elseUID = ++LabelCounter;
				int endUID  = ++LabelCounter;
				GenerateInstructions(target, exp.Right[0], config);
				target.emplace_back(JUMPIFN, elseUID);
				GenerateInstructions(target, exp.Right[1], config);
				target.emplace_back(JUMPTO,  endUID);
				target.emplace_back(LABEL,   elseUID);
				GenerateInstructions(target, exp.Right[2].Right[0], config);
				target.emplace_back(LABEL,   endUID);
			}
			else if(exp.Tok == "else")
			{
				std::cout << "Compilation error: else statement with no corresponding if statement." << std::endl;
			}
			else if(exp.Tok == "switch")
			{
				int returnUID = ++LabelCounter;
				target.emplace_back(SUBENTER, returnUID);
				for(ExprCRef subexp : exp.Right[1].Right)
				{
					if(subexp.Tok == "case")
					{
						Expression comparison;
						int skipUID = ++LabelCounter;
						comparison.Tok.Content = "==";
						comparison.Tok.Type    = Token::Operator;
						comparison.Type          = Expression::Operator;
						comparison.Left.push_back(exp.Right[0]);
						comparison.Right.push_back(subexp.Right[0]);
						GenerateInstructions(target, comparison, config);
						target.emplace_back(JUMPIFN, skipUID);
						GenerateInstructions(target, subexp.Right[1], config);
						target.emplace_back(LABEL,   skipUID);
					}
					else
						std::cout << "Compilation error: missing {} in switch statement." << std::endl;
				}
				target.emplace_back(LABEL, returnUID);
			}
			else if(exp.Tok == "return")
			{
				expandRight = true;
				code.emplace_back(RETURN);
			}
			else if(exp.Tok == "while")
			{
				int enterUID = ++LabelCounter;
				int exitUID  = ++LabelCounter;
				target.emplace_back(LABEL,   enterUID);
				GenerateInstructions(target, exp.Right[0], config);
				target.emplace_back(JUMPIFN, exitUID);
				GenerateInstructions(target, exp.Right[1], config);
				target.emplace_back(JUMPTO,  enterUID);
				target.emplace_back(LABEL,   exitUID);
			}
		}
		
		if(expandRight)
		for(ExprCRef subexp : exp.Right)
			GenerateInstructions(target, subexp, config);
		
		if(expandLeft)
		for(ExprCRef subexp : exp.Left)
			GenerateInstructions(target, subexp, config);
		
		// Append the instructions at the end of the list
		target.insert(target.end(), code.begin(), code.end());
	}
}