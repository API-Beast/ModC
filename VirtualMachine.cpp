//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "VirtualMachine.h"
#include "Configuration.h"
#include "Environment.h"

#include <cassert>
#include <iostream>

namespace ModC
{
	void VirtualMachine::execute(const InstructionList& script)
	{
	ScriptValue emptyVar;
	emptyVar.Flags   = ScriptValue::VariableRef;
	emptyVar.Type    = -2;
	emptyVar.Pointer = 0x0;
	std::map<std::string, ScriptValue>::iterator it;

	for(int i = 0; i < script.size(); ++i)
	{
		const Instruction& cmd = script[i];
		bool jump = true;
		switch(cmd.Type)
		{
			case PUSH:
				push(cmd.Value);
				break;
			case GETVAR:
				it = Variables.find(cmd.Name);
				if(it == Variables.end())
					it = Variables.emplace(cmd.Name, emptyVar).first;

				pushRef(&((*it).second));
				break;
			case EOL:
				//clearStack();
				break;
			case FUNCALL:
			{
				std::vector<int> arguments;
				arguments.reserve(cmd.NumArguments);
				for(int i = 0; i < cmd.NumArguments; ++i)
					arguments.push_back(peek(i).Type);
				
				FunctionDeclaration* fn = Environment->resolveFunctionCall(cmd.Name, arguments);
				
				if(fn)
					(*(fn->FnPtr))(this);
				else
				{
					std::cout << "RUNTIME ERROR: No function matching " <<  cmd.Name << "(";
					for(int i = 0; i < cmd.NumArguments; ++i)
					{
						if(i != 0) std::cout << ", ";
						std::cout << Config->NativeTypes[peek(i).Type].Name;
					}
					std::cout << ")" << std::endl;
				}
				break;
			}
			case OBJCALL:
			{
				std::vector<int> arguments;
				arguments.reserve(cmd.NumArguments);
				for(int i = 0; i < cmd.NumArguments; ++i)
					arguments.push_back(peek(i+1).Type);
				
				FunctionDeclaration* fn = Environment->resolveMethodCall(peek(0).Type, cmd.Name, arguments);
				
				if(fn)
					(*(fn->FnPtr))(this);
				else
				{
					std::cout << "RUNTIME ERROR: No function matching " <<  cmd.Name << "(";
					for(int i = 0; i < cmd.NumArguments; ++i)
					{
						if(i != 0) std::cout << ", ";
						std::cout << Config->NativeTypes[peek(1+i).Type].Name;
					}
					std::cout << ") in " << Config->NativeTypes[peek(0).Type].Name << std::endl;
				}
				break;
			}
			case SUBCALL:
			{
				int targetPos = -1;
				for(int j = 0; j < script.size(); ++j)
				{
					if(script[j].Type    == LABEL)
					if(script[j].LabelID == cmd.LabelID)
					{
						targetPos = j;
						break;
					}
				}
				if(targetPos != -1)
				{
					CallStack[CallStackPosition].ReturnAddress = i;
					CallStack[CallStackPosition].StackPosition = StackPosition;
					CallStackPosition++;
					i = targetPos;
				}
				break;
			}
			case SUBENTER:
			{
				int targetPos = -1;
				for(int j = i; j < script.size(); ++j)
				{
					if(script[j].Type    == LABEL)
					if(script[j].LabelID == cmd.LabelID)
					{
						targetPos = j;
						break;
					}
				}
				if(targetPos != -1)
				{
					CallStack[CallStackPosition].ReturnAddress = targetPos;
					CallStack[CallStackPosition].StackPosition = StackPosition;
					CallStackPosition++;
				}
				break;
			}
			case RETURN:
				if(CallStackPosition > 0)
				{
					ScriptValue retValue = pop();
					CallStackPosition--;
					i = CallStack[CallStackPosition].ReturnAddress;
					StackPosition = CallStack[CallStackPosition].StackPosition;
					push(retValue);
				}
				break;
			case JUMPTO:
			case JUMPIF:
			case JUMPIFN:
				     if(cmd.Type == JUMPTO ) jump =  true;
				else if(cmd.Type == JUMPIF ) jump =  pop().as<int>();
				else if(cmd.Type == JUMPIFN) jump = !pop().as<int>();
				if(jump)
				{
					int targetPos = -1;
					for(int j = 0; j < script.size(); ++j)
					{
						if(script[j].Type    == LABEL)
						if(script[j].LabelID == cmd.LabelID)
						{
							targetPos = j;
							break;
						}
					}
					if(targetPos != -1)
						i = targetPos;
				}
				break;
			case LABEL:
				// Ignore.
				break;
			default:
				std::cout << "Unimplemented Instruction " << cmd.Type << std::endl;
				break;
		}
	}
	}
	
	const ScriptValue& VirtualMachine::peek(int i)
	{
		return *Stack[StackPosition - i - 1].Value;
	}

	ScriptValue VirtualMachine::pop()
	{
		StackPosition--;
		ScriptValue copy = *(Stack[StackPosition].Value);
		if(Stack[StackPosition].Flags == 0)
			delete Stack[StackPosition].Value;
		return copy;
	}

	void VirtualMachine::push(const ScriptValue& arg)
	{
		Stack[StackPosition].Value = new ScriptValue(arg);
		Stack[StackPosition].Flags = 0;
		StackPosition++;
	}

	void VirtualMachine::clearStack()
	{
		for(int i = 0; i < StackPosition; ++i)
		if(Stack[i].Flags == 0)
			delete Stack[i].Value;
		
		StackPosition = 0;
	}

	ScriptValue* VirtualMachine::popRef()
	{
		//assert(Stack[StackPosition-1].Flags == 0);
		return Stack[--StackPosition].Value;
	}

	void VirtualMachine::pushRef(ScriptValue* arg)
	{
		Stack[StackPosition].Value = arg;
		Stack[StackPosition].Flags = 1;
		StackPosition++;
	}


}
