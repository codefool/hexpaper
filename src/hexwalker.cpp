/*
 * hexwalker.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: ghester
 */
#include <sstream>
#include "hexpaper"

namespace org {
namespace codefool {
namespace hexpaper {

// HexWalker
HexWalker::HexWalker()
: _h{ Hex{0,0} }
, _trail{ new std::vector<Hex> }
, _penDown{ false }
, _allowDups{ false }
{}

HexWalker::HexWalker( const Hex& hex, const bool allowDups )
: _h{ hex }
, _trail{ new std::vector<Hex> }
, _penDown{ false }
, _allowDups{ allowDups }
{}

HexWalker::HexWalker( const HexWalker& obj )
: _h{ obj._h }
, _trail{ obj._trail }
, _stack{ obj._stack }
, _penDown{ obj._penDown }
, _allowDups{ obj._allowDups }
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
            case 'm': push();    break;
            case 'r': pop();     break;
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

// seek the given hex from the origin hex, starting with the given face
//
//
//
//
HexWalker& HexWalker::seek( const Hex& dst )
{
    penDown();
    Offset bias = dst - _h;
    Facing dir;
    if( 0 == bias.dc() )
    {
        // go straight up or down
        dir = (bias.dr() > 0) ? _FacingD : _FacingA;
        move( dir, std::abs( bias.dr() ) );
    }
    else if( 0 == bias.dr() )
    {
        // go left or right
        while( _h != dst )
        {
            bias = dst - _h;
            if( isOdd( _h.col()) ^ settings.isOddGrid() )
                dir = ( bias.dc() > 0 ) ? _FacingB : _FacingF;
            else
                dir = ( bias.dc() > 0 ) ? _FacingC : _FacingE;
            move( dir, 1 );
        }
    }
    else
    {
        // fun fun fun
        //
        // calculate the angle from the _h to _dst and use that as the reference bearing.
        double bearing{ _h.atan( dst ) };
        std::cout << "   " << "Raw bearing from " << _h << " to " << dst << " is " << bearing << " degrees." << std::endl;

        while( _h != dst )
        {
            // choose the two hex's that are in the direction
            // How to this?
            bias = dst - _h;
            Facing f0;
            Facing f1;
            double b0, b1;
            if( bias.dr() == 0 )
            {
                // go left or right
                if( isOdd(_h.col()) ^ settings.isOddGrid() )
                    dir = ( bias.dc() > 0 ) ? _FacingC : _FacingE;
                else
                    dir = ( bias.dc() > 0 ) ? _FacingB : _FacingF;
            }
            else
            {
                if( bias.dr() > 0 )
                {
                    // dst is below _h
                    f0 = _FacingD;
                    f1 = ( bias.dc() > 0 ) ? _FacingC : _FacingE;
                }
                else
                {
                    // dst is above _h
                    f0 = _FacingA;
                    f1 = ( bias.dc() > 0 ) ? _FacingB : _FacingF;
                }

                Hex h0 = _h.at( f0 );
                Hex h1 = _h.at( f1 );
                std::cout << "   " << "Facing choices for " << _h << " are " << f0 << h0 << " and " << f1 << h1 << std::endl;

                if( dst == h0 )
                    dir = f0;
                else if( dst == h1 )
                    dir = f1;
                else
                {
                    // calculate their angles.
                    b0 = h0.atan( dst );
                    b1 = h1.atan( dst );
                    if( b0 == b1 )
                    {
                        std::cout << "***** " << h0 << " and " << h1 << " have identical angles " << b0 << " and " << b1 << "?????" << std::endl;
                    }
                    std::cout << "   " << "Raw bearing from " << h0 << " to " << dst << " is " << b0 << " degrees, delta " << std::abs( bearing - b0 ) << std::endl;
                    std::cout << "   " << "Raw bearing from " << h1 << " to " << dst << " is " << b1 << " degrees, delta " << std::abs( bearing - b1 ) << std::endl;

                    // the hex with the angle *closest* to the reference angle is the winner!
                    // FIXME: What if there is a tie?
                    dir = ( std::abs( bearing - b0 ) < std::abs( bearing - b1 ) ) ? f0 : f1;
                }
            }
            std::cout << "   " << _h << " moved dir " << dir << " to ";
            move( dir, 1 );
            std::cout << _h << std::endl;
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

HexWalker& HexWalker::push( void )
{
    _stack.push_back( _h );
    return *this;
}

HexWalker& HexWalker::pop( void )
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

Hex HexWalker::operator*() const
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

