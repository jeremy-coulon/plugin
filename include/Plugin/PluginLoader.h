
//          Copyright Jeremy Coulon 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/**
  * @file
  *
  * @mainpage
  * @author Jeremy Coulon
  * @date 2012
  *
  * @section Introduction
  *
  * This library provides a portable way of loading/unloading a C++ plugin.\n
  * The name 'plugin' refers to a dynamic library that is optional and can be loaded and unloaded at runtime.\n
  * Depending on OS, a plugin may have a different filename extension :
  * @li on Windows, a plugin has a '.dll' extension. Note that most DLLs are not plugins. A DLL must be designed specifically to be a plugin.
  * @li on Linux, a plugin usually has a '.so' extension. Note that most .so libraries are not plugins. A library must be designed specifically to be a plugin.
  *
  * \warning On Linux platform, in order to build an executable (or library) that can dynamically load a plugin you must link your executable (or library) with libdl.so.\n
  * On Windows platform, you don't need to do anything particular.
  *
  * This library provides a Plugin::IPlugin interface which can be used as a base interface for your own plugins.\n
  * You may create your specific plugin interface that inherits from Plugin::IPlugin.\n
  * Plugin::IPlugin interface depends on the 3rd party Versionning project (https://gitorious.org/cppversionning).\n
  * However, the use of Plugin::IPlugin is not mandatory and you can use the class Plugin::PluginLoader with your own provided interface type.
  *
  * @section Installation
  *
  * This library is a header-only library. That means you don't need to compile anything to use it in your own project. You can just write :
  * @code
  * #include <Plugin/PluginLoader.h>
  * @endcode
  *
  * @section License
  *
  *          Copyright Jeremy Coulon 2012.\n
  * Distributed under the Boost Software License, Version 1.0.\n
  *   (See accompanying file <a href="../LICENSE_1_0.txt">LICENSE_1_0.txt</a> or copy at\n
  *         http://www.boost.org/LICENSE_1_0.txt)
  *
  * @example MyPlugin.h
  * First, create your plugin project as a classic dynamic library.\n
  * The following plugin example contains only one class: MyPlugin.\n
  * See @ref MyPlugin.cpp for MyPlugin class implementation.
  *
  * @example MyPlugin.cpp
  * Implementation of MyPlugin class is straightforward.\n
  * See @ref MyPlugin.h for MyPlugin class declaration.
  *
  * @example main.cpp
  * Now let's see how to load your newly created plugin.\n
  * In the following code sample, we create an executable that takes the plugin path as an argument.\n
  *
  * \warning On Linux platform, in order to build an executable (or library) that can dynamically load a plugin you must link your executable (or library) with libdl.so.\n
  * On Windows platform, you don't need to do anything particular.
  *
  * If you built the provided example from source (using CMake), you can try it by going to build directory and running the following command : \n
  * On Linux 64 bits platform :
  * \verbatim ./bin/MyExe ./lib/x86_64-linux-gnu/libMyPlugin.so \endverbatim
  * On Windows Visual Studio 2010 32 bits platform :
  * \verbatim .\bin\MyExe.exe .\lib\x86-windows-vc100\MyPlugin.dll \endverbatim
  * Expected program output is : \n
  \verbatim
Plugin name    = Example
Plugin version = 1.3.4.2
  \endverbatim
  */

#pragma once

//==============
//==  Plugin  ==
//==============
#include "Plugin/PluginFactory.h"

//=============
//==  Boost  ==
//=============
#include <boost/noncopyable.hpp>

//===========
//==  STD  ==
//===========
#include <cassert>
#include <string>

//=======================
//==  OS Specific SDK  ==
//=======================
#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

/// Namespace of the Plugin library
namespace Plugin
{
    /// Loader of a concrete plugin
    /**
      * @tparam T Interface type of the concrete plugin
      */
    template<class T>
    class PluginLoader : boost::noncopyable
    {
    public:
        /// Constructor
        /**
          * If you give only a filename without any directory information,
          * it will look for a matching library in the current working directory and in directories included in the PATH environment variable.
          * If you give an absolute path to a file,
          * it will consider this name to be the plugin library.
          * If you give a relative path to a file,
          * path will be relative to the current working directory.
          * @param name Filename of the concrete plugin
          */
        PluginLoader(const std::string& name)
            : name_(name), plugin_(NULL), libHandle_(0)
        {

        }

        /// Destructor
        /**
          * Unloads the plugin if necessary.
          */
        ~PluginLoader()
        {
            unload();
        }

        /// Load the plugin
        /**
          * This method only load the dynamic library in memory. It does not instantiate the plugin facade.
          * @return True on success. False otherwise.
          */
        bool load()
        {
            if(isLoaded())
                unload();
            return loadLibrary();
        }

        /// Unload the plugin
        /**
          * If the plugin facade is instanciated, it is destroyed.
          * Then the dynamic library is unloaded from memory.
          * @return True on success. False otherwise.
          */
        bool unload()
        {
            bool res = true;
            if(isLoaded())
            {
                if(plugin_)
                {
                    callFunction<void>(PLUGIN_FACTORY_DESTROY);
                    plugin_ = NULL;
                }
                res = unloadLibrary();
                if(res)
                    libHandle_ = 0;
            }
            return res;
        }

        /// Check if the plugin is loaded.
        /**
          * @return True if the plugin is loaded. False otherwise.
          */
        bool isLoaded() const
        {
            return (libHandle_ != 0);
        }

        /// Get a pointer to the plugin interface
        /**
          * Note that this method instanciate the plugin facade singleton if it is not already created.
          * @return a valid pointer if the plugin is loaded. NULL otherwise.
          */
        T* getPluginInterfaceInstance()
        {
            if(!isLoaded())
                return NULL;
            if(!plugin_)
                plugin_ = callFunction<T*>(PLUGIN_FACTORY_CREATE);
            return plugin_;
        }

        /// Get error message
        /**
          * If any of the PluginLoader methods returns false. You can call this method to get an explanation of the error.
          * Please note that the behavior of this method is OS specific. Some OS may return an empty error message.
          */
        std::string getErrorMsg() const
        {
            return getErrorMsgImpl();
        }

    private:

#ifdef _WIN32
    /// an OS specific type that represents a library handle.
    typedef HMODULE library_handle;
    /// an OS specific type that represents a function pointer.
    typedef FARPROC generic_function_ptr;
#else
    /// an OS specific type that represents a library handle.
    typedef void * library_handle;
    /// an OS specific type that represents a function pointer.
    typedef void * generic_function_ptr;
#endif

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4191)    // 'reinterpret_cast' : unsafe conversion from 'Plugin::generic_function_ptr' to 'void (__decl*)(void)'
#endif

        /// Call without any argument the function "function_name" that is exported by the loaded plugin.
        template<class R>
        R callFunction(const char* function_name)
        {
            R (*func)();
            func = reinterpret_cast<R (*)()>(getFunction(function_name));
            assert(func);
            return (*func)();
        }

#ifdef _MSC_VER
# pragma warning (pop)
#endif

#ifdef _WIN32
        bool loadLibrary()
        {
            libHandle_ = LoadLibraryA(name_.c_str());
            return libHandle_ != 0;
        }

        bool unloadLibrary()
        {
            return FreeLibrary(libHandle_) != 0;
        }

        generic_function_ptr getFunction(const char *function_name)
        {
            return GetProcAddress(libHandle_, function_name);
        }

        std::string getErrorMsgImpl() const
        {
            return std::string();
        }
#else
        bool loadLibrary()
        {
            libHandle_ = dlopen(name_.c_str(), RTLD_LAZY);
            return libHandle_ != 0;
        }

        bool unloadLibrary()
        {
            return dlclose(libHandle_) == 0;
        }

        generic_function_ptr getFunction(const char *function_name)
        {
            return dlsym(libHandle_, function_name);
        }

        std::string getErrorMsgImpl() const
        {
            std::string result;
            char* str = dlerror();
            if(str)
                result = str;
            return result;
        }
#endif

        /// Name or path of the plugin
        std::string name_;
        /// Pointer to the plugin facade
        T* plugin_;
        /// OS specific library handle
        library_handle libHandle_;
    };
}
