
//          Copyright Jeremy Coulon 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#pragma once

//==============
//==  Plugin  ==
//==============
#include "Plugin/ExportAPI.h"

//===================
//==  Versionning  ==
//===================
#include "Versionning/Version.h"

//=============
//==  Boost  ==
//=============
#include <boost/utility.hpp>

//===========
//==  STD  ==
//===========
#include <string>

/// Namespace of the Plugin library
namespace Plugin
{
    /// Interface of a plugin
    /**
      * You are supposed to inherit and implement this interface in your plugin facade.
      * Your facade factory should be created using the macro PLUGIN_FACTORY_DECLARATION(T) and PLUGIN_FACTORY_DEFINITION(T).
      */
    class IPlugin : public boost::noncopyable
    {
    public:
        /// Get plugin name
        virtual const std::string& getPluginName() const = 0;

        /// Get plugin version
        virtual const Vers::Version& getPluginVersion() const = 0;

    protected:
        /// Destructor
        /**
          * This destructor is declared protected to strictly forbid user to delete instance of a plugin facade.
          * Construction and destruction of a plugin facade must all resides inside the boundary of the concrete plugin library.
          */
        virtual ~IPlugin() {}
    };
}

/// Declare fonctions to create and destroy your plugin facade.
/**
  * Must be used in a header file, in the global namespace.
  * @param T It is the concrete type of your plugin facade. T is not required to be in the global namespace.
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
  * @param T It is the concrete type of your plugin facade. T is not required to be in the global namespace.
  */
#define PLUGIN_FACTORY_DEFINITION(T)    \
T* globalInstance = NULL;               \
T* createPluginFacade()                 \
{                                       \
    if(!globalInstance)                 \
        globalInstance = new T();       \
    return globalInstance;              \
}                                       \
void destroyPluginFacade()              \
{                                       \
    if(globalInstance)                  \
    {                                   \
        delete globalInstance;          \
        globalInstance = NULL;          \
    }                                   \
}
