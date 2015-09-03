//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "Configuration.h"
#include "Environment.h"
#include "Fragment.h"
#include "VirtualMachine.h"

#include <string>

namespace ModC
{
	
	struct ScriptEngine
	{
		ModC::Configuration Config;
		ModC::Environment   Globals; 
		
		void           execute(const std::string& content);
		Fragment       compileFile(const std::string& path);
		VirtualMachine prepareVM();
	};

}