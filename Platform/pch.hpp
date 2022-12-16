#pragma once

#ifdef PLATFORM_IMPL
#define PLATFORM_API __declspec(dllexport)
#else
#define PLATFORM_API __declspec(dllimport)
#endif

#include <DirectXMath.h>