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

std::pair<double,double> Hex::hex2pixel( const Hex& h ) const
{
    coord_t q = h.col();
    coord_t r = h.row();
    double size = (double)settings.hexSize();
    double x;
    double y;
    switch( settings.gridType() )
    {
    case GridType::EVENQ:
        x = size * 3/2 * q;
        y = size * _SQRT3 * (r - 0.5 * (q&1));
        break;
    case GridType::EVENR:
        x = size * _SQRT3 * (q - 0.5 * (r&1));
        y = size * 3/2 * r;
        break;
    case GridType::ODDQ:
        x = size * 3/2 * q;
        y = size * _SQRT3 * (r + 0.5 * (q&1));
        break;
    case GridType::ODDR:
        x = size * _SQRT3 * (q + 0.5 * (r&1));
        y = size * 3/2 * r;
        break;
    }
    return std::pair<double,double>( x, y );
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
// http://www-cs-students.stanford.edu/~amitp/Articles/Hexagon1.html
//
double Hex::atan( const Hex& dst ) const
{
    if( *this == dst )
        return 0.0;

    std::pair<double,double> h0 = hex2pixel( *this );
    std::pair<double,double> h1 = hex2pixel( dst );

    double dx = h1.first  - h0.first;
    double dy = h0.second - h1.second;
    double t = -1;
    double ret = 0;

    if( 0.0 == dx )
    {
        // straight up or down
        ret = ( dy >= 0.0 ) ? 90.0 : 270.0;
    }
    else
    {
		t = std::atan(dy/dx) * _PI2RADS;
		if( t >= 0.0 )
			ret = ( dx > 0.0 ) ? t : t + 180.0;
		else
			ret =  ( dx > 0.0 ) ? t + 360.0 : t + 180.0;
    }
    std::cout << "****" << *this << dst
    		  << " h0:" << h0.first << ' ' << h0.second << " dx:" << dx << std::endl
    		  << " h1:" << h1.first << ' ' << h1.second << " dy:" << dy << std::endl
    		  << "  t:" << t << ' ' << ret << std::endl;
    return ret;
}

// answer the face that the origin hex (*this) is presenting to the dst hex.
// if it is ambiguous, return the double facing.
Facing Hex::bearing( const Hex& dst ) const
{
    int bearing = (int)std::round( atan( dst ) );
    // 0  ..59 : B
    // 60 ..119: A
    // 120..179: F
    // 180..239: E
    // 240..299: D
    // 300..359: C
    Facing f = _FacingB << ( bearing / 60 );
    if( 0.0 == ( bearing % 60 ) )
    {
        f.setDoubleFace( ( f >> 1 ).face() );
    }
    return f;
}

std::ostream& operator << ( std::ostream& os, const Hex& hex )
{
    os << '(' << (int)hex.col() << ',' << (int)hex.row() << ')';
    return os;
}

} // end ns hexpaper
} // end ns codefool
} // end ns org
