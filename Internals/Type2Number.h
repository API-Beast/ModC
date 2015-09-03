//  Copyright (C) 2014 Manuel Riecke <spell1337@gmail.com>
//  Licensed under the terms of the WTFPL.
//
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//  0. You just DO WHAT THE FUCK YOU WANT TO.

#pragma once

namespace ModC
{
	
	inline int Type2Number_NextValue()
	{
		static int id = 0;
		int result = id;
		++id;
		return result;
	}

	/** Returns a small value which identifies the type.
			Multiple calls with the same type return the same value. */
	template <typename T>
	int Type2Number()
	{
		static int id = Type2Number_NextValue();
		return id;
	}
	
}