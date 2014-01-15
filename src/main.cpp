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
    std::cout << "__cplusplus " << __cplusplus << std::endl;
    Hex h(5,5);
    for( int f = Facing::FACE_A; f < Facing::FACE_CNT; f++ )
    {
        Offset off = h.delta( (Facing::Face)f );
        std::cout << "Facing " << f << " " << (int)off.dc() << ',' << (int)off.dr() << std::endl;
    }
    settings.setOddGrid( false );
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
    {
        std::cout << "One hex move for each face - origin " << h;
        for( auto y : Facing() )
        {
            h.move( y );
            std::cout << ' ' << y << ':' << h;
        }
        std::cout << std::endl;
    }
    {
        std::cout << "HexWalker test" << std::endl;
        HexWalker w(h);
        w.walk(std::string("aap2c2d2e2f2a2bu2d"));
        std::cout << "- trail unsorted: ";
        for( auto h : *w.trail() )
            std::cout << h;
        std::cout << std::endl;
        w.sort();
        std::cout << "- trail sorted: ";
        for( auto h : *w.trail() )
            std::cout << h;
        std::cout << std::endl;
    }
    {
        std::cout << "hexCircField test" << std::endl;
        hexfield_t q{ hexCircField( Hex{5,5}, 2, 3 ) };
        for( auto h : *q )
            std::cout << h;
        std::cout << std::endl;
        std::cout << HexInfo{ Hex{1,1}, Hex{10,10}, q } << std::endl;
    }
    {
        std::cout << "HexWalker test" << std::endl;
        Facing f;
        for( auto hex : {Hex{5,1},Hex{8,1},Hex{8,5},Hex{5,10},Hex{1,10},Hex{1,5}} )
        {
            HexWalker w( hex );
            w.walk( std::string( "p3dme2drc2d"), f++ );
            for( auto h : *w.trail() )
                std::cout << h;
            std::cout << std::endl;

        };
    }
    {
        for( auto f : Facing() )
        {
            std::cout << "hexdrant " << f << std::endl;
            hexfield_t x = hexdrant( Hex{5,5}, Facing(f), 5 );
            std::cout << "trail ";
            for( auto h : *x )
                std::cout << h;
            std::cout << std::endl;
            std::cout << HexInfo{ Hex{1,1}, Hex{10,10}, x } << std::endl;
        }
    }

    {
        std::cout << "Facing operator test" << std::endl << "prefix++ ";
        Facing f( Facing::FACE_A );
        for( int i = Facing::FACE_A; i < Facing::FACE_CNT; ++i )
            std::cout << ++f;
        std::cout << std::endl << "postfix++ ";
        for( int i = Facing::FACE_A; i < Facing::FACE_CNT; ++i )
            std::cout << f++;
        std::cout << std::endl << "prefix-- ";
        for( int i = Facing::FACE_A; i < Facing::FACE_CNT; ++i )
            std::cout << --f;
        std::cout << std::endl << "postfix-- ";
        for( int i = Facing::FACE_A; i < Facing::FACE_CNT; ++i )
            std::cout << f--;
        std::cout << std::endl << "shift << ";
        for( int i = Facing::FACE_A; i < Facing::FACE_CNT * 2; ++i )
            std::cout << ( f << i );
        std::cout << std::endl << "shift >> ";
        for( int i = Facing::FACE_A; i < Facing::FACE_CNT * 2; ++i )
            std::cout << ( f >> i );
        std::cout << std::endl;
    }
    {
        hexfield_t y = h.neighbors();
        for( auto x : *y )
            std::cout << x;
        std::cout << std::endl;
    }

    std::cout << "=== End Run ===" << std::endl;
    return 0;
}
