#pragma once

#ifdef RENDERENGINE_IMPL
#define RENDERENGINE_API __declspec(dllexport)
#else
#define RENDERENGINE_API __declspec(dllimport)
#endif
