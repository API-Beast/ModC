//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

#include "Configuration.h"
#include "Internals/Type2Number.h"

namespace ModC
{
	
	template<typename T>
	int Configuration::registerScriptType(const std::string& name)
	{
		Type& inserted = (NativeTypes.insert(std::make_pair(Type2Number<T>(), Type())).first)->second;
		inserted.Name = name;
		inserted.MemorySize = sizeof(T);
		return Type2Number<T>();
	};

}