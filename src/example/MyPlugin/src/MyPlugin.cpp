
//          Copyright Jeremy Coulon 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "MyPlugin.h"

// Define plugin factory. Must be in the global namespace.
PLUGIN_FACTORY_DEFINITION( Example::MyPlugin )

using namespace Example;

MyPlugin::MyPlugin()
    : name_("Example"), version_(1, 3, 4, 2)
{

}

MyPlugin::~MyPlugin()
{

}

const std::string& MyPlugin::getPluginName() const
{
    return name_;
}

const Vers::Version& MyPlugin::getPluginVersion() const
{
    return version_;
}
