
//          Copyright Jeremy Coulon 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//==============
//==  Plugin  ==
//==============
#include "Plugin/PluginLoader.h"

//=============
//==  Boost  ==
//=============
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

//===========
//==  STD  ==
//===========
#include <string>

BOOST_AUTO_TEST_CASE(Nominal)
{
    boost::filesystem::path myPluginPath(MYPLUGIN_PATH);
    BOOST_REQUIRE(boost::filesystem::exists(myPluginPath));

    // Load plugin
    Plugin::PluginLoader<Plugin::IPlugin> loader(myPluginPath.native());
    BOOST_REQUIRE(loader.load());
    BOOST_REQUIRE(loader.isLoaded());

    // Create plugin facade
    Plugin::IPlugin* plugin = NULL;
    plugin = loader.getPluginInterfaceInstance();
    BOOST_REQUIRE(plugin);

    // Check plugin name
    std::string expectedName("Example");
    BOOST_CHECK_EQUAL(plugin->getPluginName(), expectedName);

    // Check plugin version
    Vers::Version expectedVersion(1, 3, 4, 2);
    BOOST_CHECK_EQUAL(plugin->getPluginVersion(), expectedVersion);

    // Unload plugin
    BOOST_CHECK(loader.unload());
}

BOOST_AUTO_TEST_CASE(PluginNotFound)
{
    std::string myPluginPath("NonExistingPath");

    // Load plugin
    Plugin::PluginLoader<Plugin::IPlugin> loader(myPluginPath);
    BOOST_CHECK(!loader.load());
    BOOST_CHECK(!loader.isLoaded());

    // Create plugin facade
    Plugin::IPlugin* plugin = NULL;
    plugin = loader.getPluginInterfaceInstance();
    BOOST_CHECK(!plugin);
}
