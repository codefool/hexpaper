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
    org::codefool::hexpaper::Hex h(5,5);
    for( int f = org::codefool::hexpaper::FACING_A; f < org::codefool::hexpaper::FACING_CNT; f++ )
    {
        org::codefool::hexpaper::Offset off = h.delta( (org::codefool::hexpaper::Facing)f );
        std::cout << "Facing " << f << " " << (int)off.dc() << ',' << (int)off.dr() << std::endl;
    }
    org::codefool::hexpaper::defaults.setOddGrid( true );
    for( int f = org::codefool::hexpaper::FACING_A; f < org::codefool::hexpaper::FACING_CNT; f++ )
    {
        org::codefool::hexpaper::Offset off = h.delta( (org::codefool::hexpaper::Facing)f );
        std::cout << "Facing " << f << " " << (int)off.dc() << ',' << (int)off.dr() << std::endl;
    }

	return 0;
}
