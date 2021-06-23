#pragma once

#ifdef  ECS_EXPORT
#define BOSSENGINE_API __declspec(dllexport) 
#else
#define BOSSENGINE_API
#endif



#include <iostream>
#include <vcruntime_typeinfo.h>
#include <cstdio>
#include "Types.h"
