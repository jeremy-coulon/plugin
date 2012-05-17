
//          Copyright Jeremy Coulon 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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

    // Instantiate a loader
    Plugin::PluginLoader<Plugin::IPlugin> loader(pluginPath);
    Plugin::IPlugin* plugin = NULL;

    // Load library in memory
    bool isLoaded = loader.load();
    if(isLoaded)
        // Create plugin facade instance
        plugin = loader.getPluginInterfaceInstance();

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
