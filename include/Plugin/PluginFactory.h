//          Copyright Jeremy Coulon 2012-2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#pragma once

//==============
//==  Plugin  ==
//==============
#include "Plugin/ExportAPI.h"

/// Function name of the Plugin factory to create Facade instance
#define PLUGIN_FACTORY_CREATE "createPluginFacade"

/// Function name of the Plugin factory to destroy Facade instance
#define PLUGIN_FACTORY_DESTROY "destroyPluginFacade"

/// Declare fonctions to create and destroy your plugin facade.
/**
  * Must be used in a header file, in the global namespace.
  * @param T It is the concrete type of your plugin facade.
  *        T is not required to be in the global namespace.
  */
#define PLUGIN_FACTORY_DECLARATION(T)               \
extern "C"                                          \
{                                                   \
PLUGIN_API T* createPluginFacade();                 \
PLUGIN_API void destroyPluginFacade();              \
}

/// Defines fonctions to create and destroy your plugin facade.
/**
  * Must be used in a cpp file.
  * Your plugin facade will behave as a singleton.
  * @param T It is the concrete type of your plugin facade.
  *        T is not required to be in the global namespace.
  */
#define PLUGIN_FACTORY_DEFINITION(T)    \
T* globalInstance = NULL;               \
T* createPluginFacade()                 \
{                                       \
    if (!globalInstance)                \
        globalInstance = new T();       \
    return globalInstance;              \
}                                       \
void destroyPluginFacade()              \
{                                       \
    if (globalInstance)                 \
    {                                   \
        delete globalInstance;          \
        globalInstance = NULL;          \
    }                                   \
}
