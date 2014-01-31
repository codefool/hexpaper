/*
 * cube.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: ghester
 */
#include "hexpaper"

namespace org {
namespace codefool {
namespace hexpaper {

// class template Cube implementation
Cube::Cube( coord_t x, coord_t y, coord_t z )
: _c( x, y, z )
{}

Cube::Cube( const Hex& hex )
: _c( 0, 0, 0 )
{
    _c = hex2cube( hex );
}

Cube::Cube( const Cube& obj )
: _c( obj._c )
{}

Cube::Cube( const CubeCoord& tri )
: _c( tri )
{}

// convert the cube coordinates to appropriate hex coords
Hex Cube::toHex() const
{
    return cube2hex( _c );
}

Cube Cube::operator*(const coord_t val) const
{
    return Cube( _c._x * val, _c._y * val, _c._z * val );
}

Cube Cube::operator+(const coord_t val) const
{
    return Cube( _c._x + val, _c._y + val, _c._z + val );
}

Cube Cube::operator+(const Cube& rhs ) const
{
    return Cube( _c._x + rhs._c._x, _c._y + rhs._c._y, _c._z + rhs._c._z );
}

Triplet<double> Cube::operator*(const double val) const
{
    return Triplet<double>( _c._x * val, _c._y * val, _c._z * val );
}

inline coord_t Cube::x() const { return _c._x; }
inline coord_t Cube::y() const { return _c._y; }
inline coord_t Cube::z() const { return _c._z; }
inline CubeCoord Cube::c() const { return _c; }

coord_t Cube::distance( const Cube& rhs ) const
{
    return (coord_t)((  std::abs( _c._x - rhs._c._x )
                      + std::abs( _c._y - rhs._c._y )
                      + std::abs( _c._z - rhs._c._z ) ) / 2 );
}

CubePath Cube::path( const Cube& dst ) const
{
    CubePath ret;
    coord_t N = distance( dst );
    for( auto i = 0; i <= N; ++i )
    {
        double iN( i / (double)N );
        Triplet<double> A = *this * ( 1.0 - iN );
        Triplet<double> B = dst * iN;
        Triplet<double> C = A + B;
        Cube R( round( C ) );
        Hex H( R );
        std::cout << "A:" << A << " B:" << B << " C:" << C << " R:" << R << " H:" << H << std::endl;
        ret.push_back( R );
    }
    return ret;
}

std::ostream& operator<<( std::ostream& os, const Cube& obj )
{
    return os << '{' << obj._c._x << ',' << obj._c._y << ',' << obj._c._z << '}';
}

CubeCoord round( const Triplet<double>& rhs )
{
    coord_t rx = (coord_t)(std::round(rhs._x));
    coord_t ry = (coord_t)(std::round(rhs._y));
    coord_t rz = (coord_t)(std::round(rhs._z));

    coord_t dx = std::abs( rx - rhs._x );
    coord_t dy = std::abs( ry - rhs._y );
    coord_t dz = std::abs( rz - rhs._z );

    if( dx > dy && dx > dz )
        rx = -ry-rz;
    else if( dy > dz )
        ry = -rx-rz;
    else
        rz = -rx-ry;

    return CubeCoord( rx, ry, rz );
}

CubeCoord hex2cube( coord_t q, coord_t r )
{
    coord_t x;
    coord_t y;
    coord_t z;

    switch( settings.gridType() )
    {
    case GridType::EVENQ:
        x = q;
        z = r - (q + (q&1)) / 2;
        y = -x-z;
        break;
    case GridType::EVENR:
        x = q - (r+(r&1)) / 2;
        z = r;
        y = -x-z;
        break;
    case GridType::ODDQ:
        x = q;
        z = r - (q - (q&1)) / 2;
        y = -x-z;
        break;
    case GridType::ODDR:
        x = q - (r - (r&1)) / 2;
        z = r;
        y = -x-z;
        break;
    }
    return CubeCoord( x, y, z );
}

CubeCoord hex2cube( const Hex& hex )
{
    return hex2cube( hex.col(), hex.row() );
}

Hex cube2hex( const coord_t x, const coord_t y, const coord_t z )
{
    coord_t r;
    coord_t q;

    switch( settings.gridType() )
    {
    case GridType::EVENQ:
        r = z + (x+(x&1)) / 2;
        q = x;
        break;
    case GridType::EVENR:
        q = x + (z+(z&1)) / 2;
        r = z;
        break;
    case GridType::ODDQ:
        q = x;
        r = z + (x-(x&1)) / 2;
        break;
    case GridType::ODDR:
        q = x + (z-(z&1)) / 2;
        r = z;
        break;
    }
    return Hex(r,q);
}

Hex cube2hex( const CubeCoord& tri )
{
    return cube2hex( tri._x, tri._y, tri._z );
}

CubePath hexpath2cube( const HexPath& hp )
{
	CubePath ret;
	for( auto n : hp )
		ret.push_back( Cube( n ) );
	return ret;
}

HexPath  cubepath2hex( const CubePath& cp )
{
	HexPath ret;
	for( auto n : cp )
		ret.push_back( Hex( n ) );
	return ret;
}

} // end ns hexpaper
} // end ns codefool
} // end ns org


