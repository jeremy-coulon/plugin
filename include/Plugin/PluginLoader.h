
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
  * @section Example
  *
  * First, create your plugin as a classic dynamic library.\n
  * The following plugin example contains only one class: MyPlugin.\n
  * Your class must inherits from an interface.\n
  * Here we inherits from Plugin::IPlugin but you can write your own interface that may (or may not) inherits from Plugin::IPlugin as well.\n
  * One important thing to don't forget here is to call the macro PLUGIN_FACTORY_DECLARATION(T).\n
  * It creates factory methods that can be called from outside of your dynamic library.\n
  * This factory implements a Singleton design pattern. There will be only one instance of MyClass during execution of the program.\n
  * \include MyPlugin.h
  *
  * Implementation of MyClass is straightforward.\n
  * The only important thing is to call the macro PLUGIN_FACTORY_DEFINITION(T).\n
  * \include MyPlugin.cpp
  *
  * Now let's see how to load your newly created plugin.\n
  * In the following code sample, we create an executable that takes the plugin path as an argument.\n
  * You first need to instantiate a Plugin::PluginLoader.\n
  * It takes your interface type as a template argument and the plugin path as a constructor argument.\n
  * Then, call Plugin::PluginLoader::load() method to actually load in memory your dynamic library.\n
  * At this time, MyPlugin class is not instantiated yet.\n
  * You have to call Plugin::PluginLoader::getPluginInterfaceInstance() to create the Singleton instance of MyPlugin.\n
  * It returns a pointer to your plugin interface type.\n
  *
  * And that's all. You can now call any method defined in your interface.\n
  *
  * \note Beware that Plugin::PluginLoader::getPluginInterfaceInstance() method does not give you ownership of Plugin::IPlugin pointer.\n
  * It means that this instance will be destroyed as soon as Plugin::PluginLoader is destroyed.\n
  * Any call to the Plugin::IPlugin pointer after Plugin::PluginLoader has been destroyed leads to undefined behavior.\n
  *
  * \include main.cpp
  *
  */

#pragma once

//==============
//==  Plugin  ==
//==============
#include "Plugin/PluginFactory.h"

//=============
//==  Boost  ==
//=============
#include <boost/utility.hpp>

//===========
//==  STD  ==
//===========
#include <cassert>
#include <string>

//=======================
//==  Specific OS SDK  ==
//=======================
#ifdef WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

/// Namespace of the Plugin library
namespace Plugin
{

#ifdef WIN32
    typedef HMODULE library_handle;
    typedef FARPROC generic_function_ptr;
#else
    /// an OS specific type that represents a library handle.
    typedef void * library_handle;
    /// an OS specific type that represents a function pointer.
    typedef void * generic_function_ptr;
#endif

    /// Loader of a concrete plugin
    /**
      * @tparam T Interface type of the concrete plugin
      */
    template<class T>
    class PluginLoader : public boost::noncopyable
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

#ifdef _MSC_VER
# pragma warning (push)
# pragma warning (disable: 4191)    // 'reinterpret_cast' : unsafe conversion from 'Plugin::generic_function_ptr' to 'void (__decl*)(void)'
#endif

        template<class R>
        R callFunction(const char* function_name)
        {
            R (*func)();
            func = reinterpret_cast<R (*)()>(getFunction(function_name));
            assert(func);
            return (*func)();
        }

        template<class R, class Arg1>
        R callFunction(const char* function_name, Arg1 arg1)
        {
            R (*func)(Arg1);
            func = reinterpret_cast<R (*)(Arg1)>(getFunction(function_name));
            assert(func);
            return (*func)(arg1);
        }

#ifdef _MSC_VER
# pragma warning (pop)
#endif

#ifdef WIN32
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

        std::string name_;
        T* plugin_;
        library_handle libHandle_;
    };
}
