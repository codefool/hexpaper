//============================================================================
// Name        : hexpaper.cpp
// Author      : G L Hester
// Version     :
// Copyright   : Copyright (C) 2014 All Rights Reserved
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include "hexpaper"
using namespace std;
using namespace org::codefool::hexpaper;

int main() {
    Hex h(5,5);
    for( int f = Facing::FACING_A; f < Facing::FACING_CNT; f++ )
    {
        Offset off = h.delta( (Facing::Face)f );
        std::cout << "Facing " << f << " " << (int)off.dc() << ',' << (int)off.dr() << std::endl;
    }
    defaults.setOddGrid( true );
    for( int f = Facing::FACING_A; f < Facing::FACING_CNT; f++ )
    {
        Offset off = h.delta( (Facing::Face)f );
        std::cout << "Facing " << f << " " << (int)off.dc() << ',' << (int)off.dr() << std::endl;
    }

    std::cout << "isEven(0)   " << org::codefool::hexpaper::isEven(0) << ' ' << org::codefool::hexpaper::isOdd(0) << std::endl;
    std::cout << "isEven(1)   " << org::codefool::hexpaper::isEven(1) << ' ' << org::codefool::hexpaper::isOdd(1) << std::endl;
    std::cout << "isEven('1') " << org::codefool::hexpaper::isEven('1') << ' ' << org::codefool::hexpaper::isOdd('1') << std::endl;
    std::cout << "isEven(std::string('1')) " << org::codefool::hexpaper::isEven(std::string("1")) << ' ' << org::codefool::hexpaper::isOdd(std::string("1")) << std::endl;

    //auto i = Facing::iterator();
    //for( auto x : i )
    //    std::cout << x << std::endl;

	return 0;
}
