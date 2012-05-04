#pragma once

#ifdef WIN32
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API
#endif
