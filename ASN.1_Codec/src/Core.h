#ifndef __REAL_CORE__
#define __REAL_CORE__

//#define REAL_DEBUG_MODE

#include <iostream>
#include "Defines.h"
#include "Platform/Platform.h"



#define DLLIMPORT __declspec(dllimport)
#define DLLEXPORT __declspec(dllexport)

#if defined(REAL_DLL_EXPORT)
#define CORE_API __declspec(dllexport)
#elif defined(REAL_DLL_IMPORT)
#define CORE_API __declspec(dllimport)
#else
#define CORE_API
#endif


#endif
