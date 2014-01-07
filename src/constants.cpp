/*
 * constants.cpp
 *
 *  Created on: Jan 7, 2014
 *      Author: hesterg
 */

#include "constants.hpp"

namespace org {
namespace codefool {
namespace hexpaper {

coord_t _dc[6] = { 0, 1, 1, 0, -1, -1 };
coord_t _dr[][6] = {
    { -1, -1, 0, 1, 0, -1 },
    { -1,  0, 1, 1, 1,  0 }
};

} // end ns hexpaper
} // end ns codefool
} // end ns org
