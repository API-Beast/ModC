//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "VirtualMachine.h"

namespace NativeWrapper
{

#define BinOpWrapper(OPERATOR, NAME)\
template<typename A, typename B>\
void NAME(ModC::VirtualMachine* vm)\
{\
	A val = vm->pop().as<A>();\
	vm->push(val OPERATOR vm->pop().as<B>());\
}

BinOpWrapper(+ , Add);
BinOpWrapper(- , Subtract);
BinOpWrapper(* , Multiply);
BinOpWrapper(/ , Divide);
BinOpWrapper(% , Modulo);
BinOpWrapper(< , LessThan);
BinOpWrapper(> , MoreThan);
BinOpWrapper(<=, LessOrEqualTo);
BinOpWrapper(>=, MoreOrEqualTo);
BinOpWrapper(==, EqualTo);
BinOpWrapper(!=, UnequalTo);
BinOpWrapper(&&, And);
BinOpWrapper(||, Or);

#define AssignOpWrapper(OPERATOR, NAME)\
template<typename A, typename B>\
void NAME(ModC::VirtualMachine* vm)\
{\
	ModC::ScriptValue* ref = vm->popRef();\
	(*ref) OPERATOR vm->pop().as<B>();\
	vm->pushRef(ref);\
}

AssignOpWrapper(= , Assign);
AssignOpWrapper(+=, AddAssign);
AssignOpWrapper(-=, SubtractAssign);
AssignOpWrapper(*=, MultiplyAssign);
AssignOpWrapper(/=, DivideAssign);

#define PrefixOpWrapper(OPERATOR, NAME)\
template<typename A>\
void NAME(ModC::VirtualMachine* vm)\
{\
	vm->push(OPERATOR vm->pop().asRef<A>());\
}

PrefixOpWrapper(--, Decrement);
PrefixOpWrapper(++, Increment);
PrefixOpWrapper(- , Negate);
PrefixOpWrapper(+ , Identity);
PrefixOpWrapper(! , Not);

#undef BinOpWrapper
#undef AssignOpWrapper
#undef PrefixOpWrapper

}