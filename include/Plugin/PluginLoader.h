#pragma once

#include "Plugin/IPlugin.h"

#include <boost/utility.hpp>

#include <cassert>
#include <string>

#ifdef WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

namespace Plugin
{

#ifdef WIN32
    typedef HMODULE library_handle;
    typedef FARPROC generic_function_ptr;
#else
    typedef void * library_handle;
    typedef void * generic_function_ptr;
#endif

    template<class T>
    class PluginLoader : public boost::noncopyable
    {
    public:
        PluginLoader(const std::string& name)
            : name_(name), plugin_(NULL), libHandle_(0)
        {

        }

        ~PluginLoader()
        {
            unload();
        }

        bool load()
        {
            if(isLoaded())
                unload();
            return loadLibrary();
        }

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

        bool isLoaded() const
        {
            return (libHandle_ != 0);
        }

        T* getPluginInterfaceInstance()
        {
            if(!isLoaded())
                return NULL;
            if(!plugin_)
                plugin_ = callFunction<T*>("createPluginFacade");
            return plugin_;
        }

        std::string getErrorMsg() const
        {
            return getErrorMsgImpl();
        }

    protected:
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
