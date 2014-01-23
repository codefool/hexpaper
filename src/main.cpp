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
    std::cout << "Odd Grid" << std::endl;
    settings.setOddGrid( true );
    Hex h(5,5);
    {
        Hex dst{1,1};
        Hex h0{3,2};
        Hex h1{2,2};
        std::cout << dst << ' ' << h0 << ' ' << dst.atan(h0) << h1 << ' ' << dst.atan(h1) << std::endl;
    }
    return 0;
    {
        for( auto dst: {Hex{5,1},Hex{5,10},Hex{3,5},Hex{7,5},Hex{6,5},Hex{6,6},Hex{4,5},Hex{4,6},Hex{6,4},Hex{6,7},Hex{4,4},Hex{4,7},Hex{4,3}})
        {
            std::cout << "atan" << h << ' ' << dst << " is " << h.atan( dst ) << std::endl;
        }

        for( int f = Facing::FACE_A; f < Facing::FACE_CNT; f++ )
        {
            Offset off = h.delta( (Facing::Face)f );
            std::cout << "Facing " << f << " " << (int)off.dc() << ',' << (int)off.dr() << std::endl;
        }
        std::cout << "Even Grid" << std::endl;
        settings.setOddGrid( false );
        for( int f = Facing::FACE_A; f < Facing::FACE_CNT; f++ )
        {
            Offset off = h.delta( (Facing::Face)f );
            std::cout << "Facing " << f << " " << (int)off.dc() << ',' << (int)off.dr() << std::endl;
        }
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
        std::cout << "Neighbors of " << h << " are ";
        for( auto x : *y )
            std::cout << x;
        std::cout << std::endl;
    }
    {
        settings.setOddGrid( true );
        Hex org{ 5, 5 };
        HexWalker w{ org };
        for( auto dst : {Hex{1,1},Hex{1,10},Hex{10,1},Hex{10,10},Hex{5,10},Hex{1,5},Hex{5,1},Hex{5,10}} )
        {
            w.clear().setOrigin( org ).seek( dst );
            std::cout << "Tracing from " << org << " to " << dst << " is " << w << std::endl;
        }
    }
    {
        settings.setOddGrid( true );
        HexWalker w;
        for( auto org : {Hex{3,3},Hex{5,4},Hex{4,5},Hex{4,4}} )
        {
            std::cout << "***** Exhaustive seek test for 5x5 grid with origin " << org << "****" << std::endl;
            for( coord_t col = 1; col < 6; ++col )
            {
                for( coord_t row = 1; row < 6; ++row )
                {
                    Hex dst{ col, row };
                    w.clear().setOrigin( org ).seek( dst );
                    std::cout << "*****" << org << ' ' << dst << ' ' << w << std::endl << std::endl;
                }
            }
        }
    }

    std::cout << "=== End Run ===" << std::endl;
    return 0;
}
