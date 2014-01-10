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
    { -1,  0, 1, 1, 1,  0 },
    { -2, -1, 1, 2, 1, -1 }
};


template <>
int isOdd<char *>(char *c)
{ return isOdd<int>(atoi(c)); }

template <>
int isOdd<std::string>(std::string s)
{ return isOdd<int>(atoi(s.c_str())); }

} // end ns hexpaper
} // end ns codefool
} // end ns org
