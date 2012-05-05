
//          Copyright Jeremy Coulon 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#pragma once

//==============
//==  Plugin  ==
//==============
#include "Plugin/IPlugin.h"

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
                    callFunction<void>("destroyPluginFacade");
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
                plugin_ = callFunction<T*>("createPluginFacade");
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
