// Distributed under GNU LESSER GENERAL PUBLIC LICENSE v2.1 (http://www.gnu.org/licenses/lgpl-2.1.html)
//
// This project lives at http://github.com/codefool/hexpaper
//
// Copyright (C) 2014 Garyl Hester
//
#include "hexpaper"
#include <cmath>
#include <sstream>

// hexpaper - library for working with hexagon grids
//
// Copyright (C) 2014 Garyl Hester
//
// defaults.cpp - implementation of the _defaults class
//
//
namespace org {
namespace codefool {
namespace hexpaper {

coord_t _dc[6] = { 0, 1, 1, 0, -1, -1 };
coord_t _dr[][6] = {
    { -1, -1, 0, 1, 0, -1 },
    { -1,  0, 1, 1, 1,  0 }
};

template <>
bool isOdd<char *>(char *c)
{ return isOdd<int>(atoi(c)); }

template <>
bool isOdd<std::string>(std::string s)
{ return isOdd<int>(atoi(s.c_str())); }

const double _PI       { 3.141592654  };
const double _RAD2DEG  { 180.0 / _PI  };
const double _SQRT3    { 1.732050808  };
const double _2SQRT3   { 2.0 * _SQRT3 };
const double _SQRT3DIV2{ _SQRT3 / 2.0 };
const double _INV2SQRT3{ 1 / _2SQRT3  };

// class Offset implementation
Offset::Offset( coord_t c, coord_t r )
: _dc( c ), _dr( r )
{}

Offset::Offset(const Offset& obj )
: _dc( obj._dc ), _dr( obj._dr )
{}

Offset::~Offset() {}

Offset Offset::operator~() const
{
	return Offset( std::abs( _dc ), std::abs( _dr ) );
}

std::ostream& operator << (std::ostream& os, const Offset& obj )
{
	os << '[' << obj.dc() << ',' << obj.dr() << ']';
	return os;
}

// class Hex implementation


// Heading

Heading::Heading( const Hex& loc, const Facing& dir )
: _loc{ loc }, _dir{ dir }
{}

Heading& Heading::turn( Facing newDir )
{
    _dir = newDir;
    return *this;
}

Heading& Heading::forward( const int distance )
{
    _loc.move(_dir, distance );
    return *this;
}

Heading& Heading::left( const int distance )
{
    _loc.move(--_dir, distance );
    return *this;
}

Heading& Heading::right( const int distance )
{
    _loc.move(++_dir, distance );
    return *this;
}

Heading& Heading::slipLeft( int distance )
{
    while( distance-- )
        _loc.move( _dir << 1, 1 );
    return *this;
}

Heading& Heading::slipRight( int distance )
{
    while( distance-- )
        _loc.move( _dir >> 1, 1 );
    return *this;
}

hexfield_t hexCircField( const Hex& org, const int innerRadius, const int outerRadius)
{
    HexWalker w( org );
    w.move( _FacingA, innerRadius-1 );
    for( int r{ innerRadius }; r <= outerRadius; ++r )
    {
        w.move( _FacingA, 1 ).penDown();
        for( auto f : Facing{_FacingC} )
            w.move( f, r );
        w.penUp();
    }
    w.sort();
    return w.trail();
}

hexfield_t hexdrant( const Hex& org, const Facing dir, const int range )
{
    // a-f (biased) pattern up to range hex's out,
    // then a-b (biased) pattern up to range
    Facing l[2] = { dir, dir << 1 };
    Facing r[2] = { dir, dir >> 1 };
    Facing m[2] = { dir >> 2, dir >> 1 };

    HexWalker w0( org );
    HexWalker w1( org );
    w0.penDown();
    w1.penDown();
    for( auto i = 0; i < range; ++i )
    {
        w0.move( l[isOdd(i)], 1 );
        w1.move( r[isOdd(i)], 1 );
        // now add all the hex's between these two
        // proceed in a c-b (biased) pattern until w0==w1
        std::cout << "left " << w0.hex() << " right " << w1.hex() << std::endl;
        short cnt{0};
        w0.mark();
        while( w0.hex() != w1.hex() )
        {
            // w0 pen is still down
            w0.move( m[isOdd(cnt++)], 1 );
            std::cout << "insert " << w0.hex() << std::endl;
        }
        w0.recall();
    }

    std::cout << "w0:";
    for( auto h : *w0.trail() )
        std::cout << h;
    std::cout << std::endl << "w1:";
    for( auto h : *w1.trail() )
        std::cout << h;
    std::cout << std::endl;

    return w0.trail();
}

// class GridCongif implementation
//
const unsigned long GridConfig::_MAGIC{0xc0def001};

GridConfig::GridConfig()
    : _gridType( GridType::ODDQ )
    , _clipping( true )
    , _hexSize( 2 )
    {}

GridConfig::~GridConfig()
{}

GridConfig& GridConfig::instance()
{
    static GridConfig instance;     // Meyers singleton...
    return instance;
}

GridConfig& GridConfig::setGridType( GridType type )
{
    _gridType = type;
    return *this;
}

const bool GridConfig::isOddGrid() const
{
    return (unsigned char)_gridType & 0x01;
}

bool GridConfig::setOddGrid( bool val )
{
    bool ret( isOddGrid() );
    if( val )
        _gridType = (GridType)((unsigned char)_gridType | 0x01);
    else
        _gridType = (GridType)((unsigned char)_gridType & 0xFE);
    return ret;
}

const bool GridConfig::isClippingOn() const
{
    return _clipping;
}

bool GridConfig::setClipping( bool val )
{
    std::swap( _clipping, val );
    return val;
}

GridType GridConfig::gridType() const
{
    return _gridType;
}

int GridConfig::isPointyTop( void ) const
{
    return ((unsigned char)_gridType & 0x2) ? 1 : 0;
}

int GridConfig::isFlatTop( void ) const
{
    return ((unsigned char)_gridType & 0x2) ? 0 : 1;
}

int GridConfig::hexSize() const
{
    return _hexSize;
}

// when equating two configurations, then only thing that matters
// is the gridtype. If those are the same, everything else will
// work, but if they differ nothing will...
bool GridConfig::operator==( const GridConfig& rhs ) const
{
    return _gridType == rhs._gridType;
}

bool GridConfig::operator!=(const GridConfig& rhs ) const
{
    return !(*this == rhs);
}

GridConfig & defaultGridConfig = GridConfig::instance();

} // end ns hexpaper
} // end ns codefool
} // end ns org




