//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include <string>
#include <vector>

namespace ModC
{
	
	struct VirtualMachine;

	using NativeFunction = void (*)(VirtualMachine*);

	struct FunctionDeclaration
	{
		std::string Name;
		std::vector<int> Parameters;
		int ReturnType = -1;
		
		NativeFunction FnPtr = nullptr;
	};

}