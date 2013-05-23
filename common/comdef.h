#pragma once

#ifdef WIN32
	#include <windows.h>
#else
	#include "winerror.h"
	typedef unsigned int 		UINT;
#endif

#include "config.h"
