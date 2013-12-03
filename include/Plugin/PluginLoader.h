//          Copyright Jeremy Coulon 2012-2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/** @file */

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
#include <cstddef>
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
    class PluginLoader : private boost::noncopyable
    {
    public:
        /// Constructor
        /**
          * If you give only a filename without any directory information,
          * it will look for a matching library in the current working directory
          * and in directories included in the PATH environment variable.
          * If you give an absolute path to a file,
          * it will consider this name to be the plugin library.
          * If you give a relative path to a file,
          * path will be relative to the current working directory.
          * @param name Filename of the concrete plugin
          */
        explicit PluginLoader(const std::string& name = "")
            : name_(name),
              plugin_(NULL),
              libHandle_(0)
        {
            // Empty
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
          * This method only load the dynamic library in memory.
          * It does not instantiate the plugin facade.
          * @return True on success. False otherwise.
          */
        bool load()
        {
            if (isLoaded())
                unload();
            if (name_.empty())
                return false;
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
            if (isLoaded())
            {
                if (plugin_)
                {
                    callFunction<void>(PLUGIN_FACTORY_DESTROY);
                    plugin_ = NULL;
                }
                res = unloadLibrary();
                if (res)
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
          * Note that this method instanciate the plugin facade singleton
          * if it is not already created.
          * @return a valid pointer if the plugin is loaded. NULL otherwise.
          */
        T* getPluginInstance()
        {
            if (!isLoaded())
                return NULL;
            if (!plugin_)
                plugin_ = callFunction<T*>(PLUGIN_FACTORY_CREATE);
            return plugin_;
        }

        /// Get error message
        /**
          * If any of the PluginLoader methods returns false,
          * you can call this method to get an explanation of the error.
          * Please note that the behavior of this method is OS specific.
          * Some OS may return an empty error message.
          */
        const std::string& getErrorMsg() const
        {
            return errorMsg_;
        }

        /**
         * @brief Get the plugin filename
         * @return filename of the concrete plugin
         */
        const std::string& getPluginName() const
        {
            return name_;
        }

        /**
         * @brief Set the plugin filename
         * @param name filename of the concrete plugin
         */
        void setPluginName(const std::string& name)
        {
            name_ = name;
        }

    private:

#ifdef _WIN32
    // an OS specific type that represents a library handle.
    typedef HMODULE library_handle;
    // an OS specific type that represents a function pointer.
    typedef FARPROC generic_function_ptr;
#else
    // an OS specific type that represents a library handle.
    typedef void * library_handle;
    // an OS specific type that represents a function pointer.
    typedef void * generic_function_ptr;
#endif

#ifdef _MSC_VER
# pragma warning (push)
// 'reinterpret_cast': unsafe conversion
// from 'Plugin::generic_function_ptr'
// to 'void (__decl*)(void)'
# pragma warning (disable: 4191)
#endif

        // Call without any argument the function "function_name"
        // that is exported by the loaded plugin.
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
#else
        bool loadLibrary()
        {
            libHandle_ = dlopen(name_.c_str(), RTLD_LAZY);
            if (!libHandle_)
                saveErrorMsg();
            return libHandle_ != NULL;
        }

        bool unloadLibrary()
        {
            int res = dlclose(libHandle_);
            if (res != 0)
                saveErrorMsg();
            return res == 0;
        }

        generic_function_ptr getFunction(const char* function_name)
        {
            void* result = dlsym(libHandle_, function_name);
            if (!result)
                saveErrorMsg();
            return result;
        }

        void saveErrorMsg()
        {
            // Save error message
            char* str = dlerror();
            if (str)
                errorMsg_ = str;
            // dlerror() must be called twice
            // in order to avoid memory leaks
            str = dlerror();
            assert(!str);
        }
#endif

        // Name or path of the plugin
        std::string name_;
        // Pointer to the plugin facade
        T* plugin_;
        // OS specific library handle
        library_handle libHandle_;
        // Error message
        std::string errorMsg_;
    };
}
