//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#include "ModC.h"
#include <Springbok/Platform.hpp>

using namespace ModC;

void Log(VirtualMachine* vm)
{
	std::cout << vm->pop().as<std::string>() << std::endl;
}

void LogI(VirtualMachine* vm)
{
	std::cout << vm->pop().as<int>() << std::endl;
}

int main()
{
	ScriptEngine modc = ModScript();
	modc.Globals.registerFunction("Log", &Log, {Type2Number<std::string>()});
	modc.Globals.registerFunction("Log", &LogI, {Type2Number<int>()});
	modc.execute("a = 5; Log(a+2);");
}