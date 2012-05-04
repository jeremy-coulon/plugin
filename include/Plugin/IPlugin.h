#pragma once

#include "Plugin/ExportAPI.h"
#include "Versionning/Version.h"

#include <boost/utility.hpp>

#include <string>

namespace Plugin
{
    class IPlugin : public boost::noncopyable
    {
    public:
        virtual const std::string& getPluginName() const = 0;
        virtual const Versionning::Version& getPluginVersion() const = 0;
    protected:
        virtual ~IPlugin() {}
    private:
    };
}

#define PLUGIN_FACTORY_DECLARATION(T)               \
extern "C"                                          \
{                                                   \
T* PLUGIN_API createPluginFacade();                 \
void PLUGIN_API destroyPluginFacade();              \
}

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
