/*
 * hexwalker.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: ghester
 */
// Distributed under GNU LESSER GENERAL PUBLIC LICENSE v2.1 (http://www.gnu.org/licenses/lgpl-2.1.html)
//
// This project lives at http://github.com/codefool/hexpaper
//
// Copyright (C) 2014 Garyl Hester
//
#include <sstream>
#include "hexpaper"

namespace org {
namespace codefool {
namespace hexpaper {

// HexWalker
HexWalker::HexWalker( const GridConfig& cfg )
: _h{ Hex{0,0,cfg} }
, _trail{ new std::vector<Hex> }
, _penDown{ false }
, _allowDups{ false }
{}

HexWalker::HexWalker( const Hex& hex, const bool penDown, const bool allowDups )
: _h{ hex }
, _trail{ new std::vector<Hex> }
, _penDown{ penDown }
, _allowDups{ allowDups }
{}

HexWalker::HexWalker( const HexWalker& obj )
: _h{ obj._h }
, _trail{ obj._trail }
, _stack{ obj._stack }
, _penDown{ obj._penDown }
, _allowDups{ obj._allowDups }
{}

HexWalker::~HexWalker()
{}

HexWalker& HexWalker::setOrigin( const Hex& hex )
{
    _h = hex;
    penUp();
    return *this;
}

// walk the hex around
//     n   - list of digits - repeat the following command n times.
//     A-F - move pen in that direction relative to face.
//     P   - pen down (start recording all hex's from current hex on.)
//     U   - pen up   (stop recording hex's.)
//     M   - mark - push this hex on the stack.
//     R   - recall - pop hex off the stack (and make that hex the current hex.)
HexWalker& HexWalker::walk( std::string&& path, const Facing& bias )
{
    std::transform( path.begin(), path.end(), path.begin(), ::tolower );
    int cnt{0};
    for( char c : path )
    {
        if( '0' <= c && c <= '9' )
        {
            cnt = ( cnt * 10 ) + c - '0';
        }
        else if( 'a' <= c && c <= 'f' )
        {
            move( Facing(c) >> bias, cnt );
            cnt = 0;
        }
        else
            switch( c )
            {
            case 'p': penDown(); break;
            case 'u': penUp();   break;
            case 'm': mark();    break;
            case 'r': recall();  break;
            default:
            {
                std::stringstream msg;
                msg << "Unknown command issued to HexWalker::walk - '" << c << "'";
                throw msg.str();
            }
            }
    }
    return *this;
}

HexWalker& HexWalker::penUp( void )
{
    _penDown = false;
    return *this;
}

HexWalker& HexWalker::penDown( void )
{
    _penDown = true;
    return *this;
}

HexWalker& HexWalker::mark( void )
{
    _stack.push_back( _h );
    return *this;
}

HexWalker& HexWalker::recall( void )
{
    _h = _stack.back();
    _stack.pop_back();
    return *this;
}

HexWalker& HexWalker::clear( void )
{
    _stack.clear();
    _trail->clear();
    penUp();
    return *this;
}

HexWalker& HexWalker::move( const Facing& dir, int cnt, const Facing& bias )
{
    while( cnt-- )
    {
        _h.move( dir, 1, bias );
        if( _penDown )
        {
            if( _allowDups || _trail->end() == std::find( _trail->begin(), _trail->end(), _h ) )
                _trail->push_back( _h );
        }
    }
    return *this;
}

// sort the trail
HexWalker& HexWalker::sort( void )
{
    std::sort(_trail->begin(), _trail->end(),
        [](Hex a, Hex b)
        {
            if( a.col() == b.col() )
                return a.row() < b.row();
            else
                return a.col() < b.col();
        }
    );
    return *this;
}

hexfield_t HexWalker::trail( void )
{
    return _trail;
}

const Hex& HexWalker::hex( void ) const
{
    return _h;
}

Hex& HexWalker::operator*()
{
	return _h;
}

std::ostream& operator<<( std::ostream& os, const HexWalker& hw )
{
    for( auto h : *hw._trail )
        os << h;
    return os;
}

} // end ns hexpaper
} // end ns codefool
} // end ns org

