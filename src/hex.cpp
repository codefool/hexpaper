/*
 * hex.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: ghester
 */

#include "hexpaper"

namespace org {
namespace codefool {
namespace hexpaper {

Hex::Hex( coord_t col, coord_t row )
: _col{col}, _row{row}
{}

Hex::Hex( const Hex& obj )
: _col{ obj._col }, _row{ obj._row }
{}

Hex::Hex( Offset& off )
: _col{ off.dc() }, _row{ off.dr() }
{}

Hex::Hex( const Cube& obj )
: Hex( obj.toHex() )
{}


Hex::~Hex() {}

// return an Offset which contains the delta col and row
// for this Hex object at the given facing
Offset Hex::delta( const Facing& f ) const
{
    coord_t dc = _dc[f];
    coord_t dr = _dr[ isOdd( _col ) ^ settings.isOddGrid() ][f];
    return Offset(dc,dr);
}

Hex& Hex::move( const Facing& dir, int distance, const Facing& bias )
{
    while( distance-- )
    {
        Offset off = delta( dir >> bias );
        _col += off.dc();
        _row += off.dr();
    }
    return *this;
}

Hex& Hex::operator+=( const Offset& off )
{
    _col += off.dc();
    _row += off.dr();
    return *this;
}

Hex Hex::operator+( const Offset& off )
{
    return Hex{ (coord_t)(_col + off.dc()), (coord_t)(_row + off.dr()) };
}

Hex Hex::at( const Facing& f )
{
    Hex ret{ *this };
    return ret + delta( f );
}

Offset Hex::operator-( const Hex& rhs ) const
{
    return Offset( col() - rhs.col(), row() - rhs.row() );
}

bool Hex::operator==(const Hex& rhs ) const
{
    return _col == rhs._col && _row == rhs._row;
}

bool Hex::operator!=(const Hex& rhs ) const
{
    return !( *this == rhs );
}

// Relational operators
//
// Given two hexes a and b:
// - If a is closer to the top edge then a < b.
// - If a is closer to the left edge, then a < b.
bool Hex::operator> (const Hex& rhs ) const
{
    // FIXME: This is dependent on grid domination
    return _col < rhs._col || _row < rhs._row;
}

bool Hex::operator< (const Hex& rhs ) const
{
    // FIXME: This is dependent on grid domination
    return _col > rhs._col || _row > rhs._row;
}

bool Hex::operator>=(const Hex& rhs ) const
{
    return *this > rhs || *this == rhs;
}

bool Hex::operator<=(const Hex& rhs ) const
{
    return *this < rhs || *this == rhs;
}

Cube Hex::toCube() const
{
    return Cube(*this);
}

// return a field of all the hex's immediately surrounding this one.
hexfield_t Hex::neighbors() const
{
    return hexCircField( *this, 1, 1 );
}

// atan - return angle (in degrees) of this hex to the dst hex.
//
// This is based on the principle that an individual hexagon on the grid
// is a collection of six equilateral triangles joined at a single vertex,
// then we can get a Cartesian "angle" on the hex grid. Assign the length
// of a side of each triangle 2, and split the equilateral into two right
// triangles with sides base=1, h=sqrt(3), and hyp=2. Using these, and
// the offsets between here and there, we can deduce real-world distances.
//
double Hex::atan( const Hex& dst ) const
{
    Offset bias{ dst - *this };
    double ret;
    if( 0 == bias.dc() )
    {
        ret = ( bias.dr() < 0 ) ? 90.0 : 270.0;
    }
    else if( 0 == bias.dr() && isEven( bias.dc() ) )
    {
        ret = ( bias.dc() < 0 ) ? 180.0 : 0.0;
    }
    else
    {
        double adj{ 0.0 };
        if( isOdd( col() ) ^ isOdd( dst.col() ) )
        {
        	if( isOdd( col() ) ^ settings.isOddGrid() )
        		adj = ( bias.dr() <= 0 ) ? -0.5 : 0.5;
        	else
        		adj = ( bias.dr() < 0 ) ?  -0.5 : 0.5;
        }
        std::cout << 'c' << isOdd( dst.col() ) << 'r' << isOdd( row() ) << 'g' << settings.isOddGrid() << bias << ' '
        		  << *this << ':' << dst << " adj:" << adj;
        double ddc = (double)bias.dc() * 3;
        double ddr = ( (double)bias.dr() + adj ) * TWOSQRT3;
        ret = std::atan( ddr / ddc ) * PI2RADS;
        std::cout << " ddc:" << ddc << " ddr:" << ddr << " atan:" << ret;
        if( bias.dc() > 0 )
        {
        	if( bias.dr() >= 0 )
        		ret = 360 - ret;
        	else
        		ret = 0 - ret;
        }
        else
        {
        	ret = 180 - ret;
        }
        std::cout << ' ' << ret;
        std::cout << std::endl;
    }
    return ret;
}

std::ostream& operator << ( std::ostream& os, const Hex& hex )
{
    os << '(' << (int)hex.col() << ',' << (int)hex.row() << ')';
    return os;
}

} // end ns hexpaper
} // end ns codefool
} // end ns org
