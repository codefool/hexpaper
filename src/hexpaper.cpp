//============================================================================
// Name        : hexpaper.cpp
// Author      : G L Hester
// Version     :
// Copyright   : Copyright (C) 2014 All Rights Reserved
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "hexpaper"
using namespace std;

int main() {
    org::codefool::hexpaper::defaults.setOddGrid( true );

    cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
