//============================================================================
// Name        : hexpaper.cpp
// Author      : G L Hester
// Version     :
// Copyright   : Copyright (C) 2014 All Rights Reserved
// Description : Hello World in C++, Ansi-style
//============================================================================
// Distributed under GNU LESSER GENERAL PUBLIC LICENSE v2.1 (http://www.gnu.org/licenses/lgpl-2.1.html)
//
// This project lives at http://github.com/codefool/hexpaper
//
// Copyright (C) 2014 Garyl Hester
//
#include <iostream>
#include <string>
#include "hexpaper"
using namespace std;
using namespace org::codefool::hexpaper;

int main() {
    {
        Triplet<double> a(1.5, 2.1, -3.8 );
        Triplet<coord_t> b = round(a);
        std::cout << b << std::endl;
        Hex org{ 5, 5 };
        Hex dst{ 1, 1 };
        CubePath path = Cube(org).path(dst);
        HexPath hpath = cubepath2hex( path );

        std::cout << "Path from " << org << " to " << dst << std::endl;
        for( auto x : path )
            std::cout << x;
        std::cout << std::endl;
        for( auto x : hpath )
            std::cout << x;
        std::cout << std::endl;
    }

    std::cout << "Odd Grid" << std::endl;
    defaultGridConfig.setOddGrid( true );
    Hex h(5,5);
    {
        Hex dst{1,1};
        Hex h0{3,2};
        Hex h1{2,2};
        h1.atan(Hex{1,2});
        std::cout << dst << ' ' << h0 << ' ' << h0.atan(dst) << h1 << ' ' << h1.atan(dst) << std::endl;
    }
    {
    	Hex org{4,4};
    	for( coord_t c : {3,4,5} )
    		for( coord_t r : {2,3,4,5,6})
    		{
    			Hex dst{c,r};
    			std::cout << org << ' ' << dst << ' ' << org.atan( dst ) << std::endl;
    		}
    }
    {
        for( auto dst: {Hex{5,1},Hex{5,10},Hex{3,5},Hex{7,5},Hex{6,5},Hex{6,6},Hex{4,5},Hex{4,6},Hex{6,4},Hex{6,7},Hex{4,4},Hex{4,7},Hex{4,3}})
        {
            std::cout << "atan" << h << ' ' << dst << " is " << h.atan( dst ) << std::endl;
        }

        for( int f = FACE_A; f < FACE_CNT; f++ )
        {
            Offset off = h.delta( (Face)f );
            std::cout << "Facing " << f << " " << (int)off.dc() << ',' << (int)off.dr() << std::endl;
        }
        std::cout << "Even Grid" << std::endl;
        defaultGridConfig.setOddGrid( false );
        for( int f = FACE_A; f < FACE_CNT; f++ )
        {
            Offset off = h.delta( (Face)f );
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
        Facing f( FACE_A );
        for( int i = FACE_A; i < FACE_CNT; ++i )
            std::cout << ++f;
        std::cout << std::endl << "postfix++ ";
        for( int i = FACE_A; i < FACE_CNT; ++i )
            std::cout << f++;
        std::cout << std::endl << "prefix-- ";
        for( int i = FACE_A; i < FACE_CNT; ++i )
            std::cout << --f;
        std::cout << std::endl << "postfix-- ";
        for( int i = FACE_A; i < FACE_CNT; ++i )
            std::cout << f--;
        std::cout << std::endl << "shift << ";
        for( int i = FACE_A; i < FACE_CNT * 2; ++i )
            std::cout << ( f << i );
        std::cout << std::endl << "shift >> ";
        for( int i = FACE_A; i < FACE_CNT * 2; ++i )
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
        defaultGridConfig.setOddGrid( true );
        for( auto org : {Hex{3,3},Hex{5,4},Hex{4,5},Hex{4,4}} )
        {
            std::cout << std::endl
                      << "***** Exhaustive path test for 6x6 grid with origin " << org << "****" << std::endl;
            for( coord_t col = 1; col < 7; ++col )
            {
                for( coord_t row = 1; row < 7; ++row )
                {
                    Hex dst{ col, row };
                    std::cout << "*****" << org << dst << std::endl;
                    //HexPath path = cubepath2hex( Cube( org ).path( dst ) );
                    //for( auto h : path )
                    //	std::cout << h;
                    double t = org.atan( dst );
                    std::cout << " atan:" << t
                              << " bearing:" << org.bearing( dst )
                              << std::endl;
                }
            }
        }
    }

    Cube cube(0,0,0);

    std::cout << "=== End Run ===" << std::endl;
    return 0;
}
