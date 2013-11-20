//          Copyright Jeremy Coulon 2012-2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#pragma once

/**
  *  @def PLUGIN_API Used to define exported library interface.
  */

#ifdef WIN32
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API
#endif
