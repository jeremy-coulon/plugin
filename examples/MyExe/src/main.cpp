
//          Copyright Jeremy Coulon 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//[main_cpp_file
//==============
//==  Plugin  ==
//==============
#include "Plugin/PluginLoader.h"
#include "Plugin/IPlugin.h"

//===========
//==  STD  ==
//===========
#include <iostream>

int main(int argc, char** argv)
{
    // Check program arguments
    if(argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " ./path/myPlugin.<ext>" << std::endl;
        return 0;
    }

    // Path to the plugin we want to load
    std::string pluginPath(argv[1]);

    // You first need to instantiate a Plugin::PluginLoader.
    // It takes your interface type as a template argument and the plugin path as a constructor argument.
    Plugin::PluginLoader<Plugin::IPlugin> loader(pluginPath);
    Plugin::IPlugin* plugin = NULL;

    // Then, call Plugin::PluginLoader::load() method to actually load in memory your dynamic library.
    bool isLoaded = loader.load();
    // At this time, MyPlugin class is not instantiated yet.
    if(isLoaded)
        // You have to call Plugin::PluginLoader::getPluginInterfaceInstance() to create the Singleton instance of MyPlugin.
        // It returns a pointer to your plugin interface type.
        plugin = loader.getPluginInterfaceInstance();

    // Beware that Plugin::PluginLoader::getPluginInterfaceInstance() method does not give you ownership of Plugin::IPlugin pointer.
    // It means that this instance will be destroyed as soon as Plugin::PluginLoader is destroyed.
    // Any call to the Plugin::IPlugin pointer after Plugin::PluginLoader has been destroyed leads to undefined behavior.

    // And that's all. You can now call any method defined in your interface.

    if(plugin)
    {
        std::cout << "Plugin name    = " << plugin->iGetPluginName() << std::endl;
        std::cout << "Plugin version = " << plugin->iGetPluginVersion() << std::endl;
    }
    else
    {
        std::cout << "Failed to load plugin = " << pluginPath << std::endl;
        std::cout << "Reason = " << loader.getErrorMsg() << std::endl;
    }

    return 0;
}
//]
