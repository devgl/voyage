#pragma once

#ifdef FRAMEWORK_IMPL
#define FRAMEWORK_API __declspec(dllexport)
#else
#define FRAMEWORK_API __declspec(dllimport)
#endif
