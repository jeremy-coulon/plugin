
//          Copyright Jeremy Coulon 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//==============
//==  Plugin  ==
//==============
#include "Plugin/IPlugin.h"
#include "Plugin/PluginFactory.h"

namespace Example
{
    class MyPlugin : public Plugin::IPlugin
    {
    public:
        /// Constructor
        MyPlugin();
        /// Destructor
        virtual ~MyPlugin();

        /// IPlugin interface implementation
        ///@{
        virtual const std::string& iGetPluginName() const;
        virtual const Vers::Version& iGetPluginVersion() const;
        ///@}

    protected:
        std::string name_;
        Vers::Version version_;
    };
}

// Declare plugin factory. Must be in the global namespace.
PLUGIN_FACTORY_DECLARATION( Example::MyPlugin )
