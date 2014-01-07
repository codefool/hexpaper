//============================================================================
// Name        : hexpaper.cpp
// Author      : G L Hester
// Version     :
// Copyright   : Copyright (C) 2014 All Rights Reserved
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "defaults.hpp"
using namespace std;

namespace org {
namespace codefool {
namespace hexpaper {
} // end ns hexpaper
} // end ns codefool
} // end ns org

int main() {
    org::codefool::hexpaper::defaults.setOddGrid( true );

    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
