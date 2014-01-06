//============================================================================
// Name        : hexpaper.cpp
// Author      : G L Hester
// Version     :
// Copyright   : Copyright (C) 2014 All Rights Reserved
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

namespace org {
namespace codefool {
namespace hexpaper {

struct _defaults
{
    bool oddGrid;   // true if grid is odd-oriented, false if even
    _defaults()
    : oddGrid(false)
    {}
};

static _defaults defaults;

} // end ns hexpaper
} // end ns codefool
} // end ns org

int main() {
    org::codefool::hexpaper::defaults.oddGrid = true;

    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
