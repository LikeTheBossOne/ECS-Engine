#pragma once

#ifdef  ECS_EXPORT
#define ECS_API __declspec(dllexport) 
#else
#define ECS_API 
#endif



#include <iostream>
#include <vcruntime_typeinfo.h>
#include <cstdio>
#include "Types.h"
