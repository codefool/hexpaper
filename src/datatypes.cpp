/*
 * datatypes.cpp
 *
 *  Created on: Jan 7, 2014
 *      Author: hesterg
 */
#include "datatypes.hpp"
#include "constants.hpp"
#include "settings.hpp"

namespace org {
namespace codefool {
namespace hexpaper {

Offset::Offset( coord_t c, coord_t r )
: _dc( c ), _dr( r )
{}

Offset::Offset(const Offset& obj )
: _dc( obj._dc ), _dr( obj._dr )
{}

Offset::~Offset() {}


Hex::Hex( coord_t col, coord_t row)
: _col{col}, _row{row}
{}

Hex::Hex( Offset& off )
: _col{ off.dc() }, _row{ off.dr() }
{}

Hex::~Hex() {}

// return an Offset which contains the delta col and row
// for this Hex object at the given facing
Offset Hex::delta( Facing::Face f ) const
{
    coord_t dc = _dc[f];
    coord_t dr = _dr[ isOdd( _col ) ^ settings.gridOrientation() ][f];
    return Offset(dc,dr);
}

Hex& Hex::move( Facing::Face dir, int distance )
{
    while( distance-- )
    {
        Offset off = delta( dir );
        _col += off.dc();
        _row += off.dr();
    }
    return *this;
}

Hex& Hex::operator+( const Offset& off )
{
    _col += off.dc();
    _row += off.dr();
    return *this;
}

std::ostream& operator << ( std::ostream& os, const Hex& hex )
{
    os << '(' << (int)hex.col() << ',' << (int)hex.row() << ')';
    return os;
}

// HexWalker
HexWalker::HexWalker( const Hex& hex )
: _h{ hex }, _penDown{ false }
{}

HexWalker::HexWalker( const HexWalker& obj )
: _h{ obj._h }
, _trail{ obj._trail }
, _stack{ obj._stack }
,  _penDown{ obj._penDown }
{}


// walk the hex around
//     n   - list of digits - repeat the following command n times.
//     A-F - move pen in that direction relative to face.
//     P   - pen down (start recording all hex's from current hex on.)
//     U   - pen up   (stop recording hex's.)
//     M   - mark - push this hex on the stack.
//     R   - recall - pop hex off the stack (and make that hex the current hex.)
void HexWalker::walk( std::string&& path )
{
    std::transform(path.begin(), path.end(), path.begin(), ::tolower);
    int cnt{0};
    for( char c : path )
    {
        if( '0' <= c && c <= '9' )
        {
            cnt = ( cnt * 10 ) + c - '0';
        }
        else if( 'a' <= c && c <= 'f' )
        {
            move( (Facing::Face)( c - 'a'), cnt );
            cnt = 0;
        }
        else
            switch( c )
            {
            case 'p': penDown(); break;
            case 'u': penUp();   break;
            case 'm': push();    break;
            case 'r': pop();     break;
            }
    }
}

void HexWalker::penUp( void )
{
    _penDown = false;
}

void HexWalker::penDown( void )
{
    _penDown = true;
}

void HexWalker::push( void )
{
    _stack.push_back( _h );
}

void HexWalker::pop( void )
{
    _h = _stack.back();
    _stack.pop_back();
}

void HexWalker::move( Facing::Face dir, int cnt )
{
    if( !cnt ) cnt=1;
    while( cnt-- )
    {
        _h.move(dir,1);
        if( _penDown )
            _trail.push_back( _h );
    }
}

// sort the trail
void HexWalker::sort( void )
{
    std::sort(_trail.begin(), _trail.end(),
        [](Hex a, Hex b)
        {
            if( a.col() == b.col() )
                return a.row() < b.row();
            else
                return a.col() < b.col();
        }
    );
}

std::vector<Hex> HexWalker::trail( void ) const
{
    return _trail;
}

std::vector<Hex> hexCircField( const Hex& org, const int innerRadius, const int outerRadius)
{
    HexWalker w( org );
    w.move( Facing::FACE_A, innerRadius-1 );
    for( int r{ innerRadius }; r <= outerRadius; ++r )
    {
        w.move( Facing::FACE_A, 1 );
        w.penDown();
        for( auto f : Facing{Facing::FACE_C} )
            w.move( f, r );
        w.penUp();
    }
    //w.sort();
    return w.trail();
}

} // end ns hexpaper
} // end ns codefool
} // end ns org
