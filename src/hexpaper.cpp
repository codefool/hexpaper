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
    for( int f = Facing::FACE_A; f < Facing::FACE_CNT; f++ )
    {
        Offset off = h.delta( (Facing::Face)f );
        std::cout << "Facing " << f << " " << (int)off.dc() << ',' << (int)off.dr() << std::endl;
    }
    settings.setOddGrid( true );
    for( int f = Facing::FACE_A; f < Facing::FACE_CNT; f++ )
    {
        Offset off = h.delta( (Facing::Face)f );
        std::cout << "Facing " << f << " " << (int)off.dc() << ',' << (int)off.dr() << std::endl;
    }

    std::cout << "isEven(0)   " << org::codefool::hexpaper::isEven(0) << ' ' << org::codefool::hexpaper::isOdd(0) << std::endl;
    std::cout << "isEven(1)   " << org::codefool::hexpaper::isEven(1) << ' ' << org::codefool::hexpaper::isOdd(1) << std::endl;
    std::cout << "isEven('1') " << org::codefool::hexpaper::isEven('1') << ' ' << org::codefool::hexpaper::isOdd('1') << std::endl;
    std::cout << "isEven(std::string('1')) " << org::codefool::hexpaper::isEven(std::string("1")) << ' ' << org::codefool::hexpaper::isOdd(std::string("1")) << std::endl;

    for( auto y : Facing() )
    {
        std::cout << "iterating face " << y << std::endl;
        for( auto x : Facing{ y } )
            std::cout << x << std::endl;
    }

    std::cout << h;
    for( auto y : Facing() )
    {
        h.move( y );
        std::cout << y << ':' << h;
    }
    std::cout << std::endl;
    settings.setOddGrid( false );
    HexWalker w(h);
    w.walk(std::string("aap2c2d2e2f2a2bu2d"));
    for( auto h : w.trail() )
        std::cout << h;
    std::cout << std::endl;
    w.sort();
    for( auto h : w.trail() )
        std::cout << h;
    std::cout << std::endl;

    std::vector<Hex> q = hexCircField( Hex{5,5}, 2, 3 );
    for( auto h : q )
        std::cout << h;
    std::cout << std::endl;

	return 0;
}
