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

const double PI      { 3.141592654 };
const double PI2RADS { 180.0 / PI  };
const double SQRT3   { 1.732050808 };
const double TWOSQRT3{ 2 * SQRT3   };

const Facing _FacingA{ Facing::FACE_A };
const Facing _FacingB{ Facing::FACE_B };
const Facing _FacingC{ Facing::FACE_C };
const Facing _FacingD{ Facing::FACE_D };
const Facing _FacingE{ Facing::FACE_E };
const Facing _FacingF{ Facing::FACE_F };
const Facing _FacingX{ Facing::FACE_CNT };

Facing::Facing( const Face f )
: _face(f)
{}

Facing::Facing( const char face )
: _face{ FACE_A }
{
    char c{(char)::tolower(face)};
    if( 'a' <= c && c <= 'f' )
        _face = (Face)(c - 'a');
}

Facing::Facing( const Facing& obj )
: _face{ obj._face }
{}

Facing Facing::operator+( const Facing& rhs ) const
{
    return Facing{ (Facing::Face)( ( _face + rhs.face() ) % FACE_CNT ) };
}

Facing Facing::operator-( const Facing& rhs ) const
{
    int f = _face - rhs.face();
    while( f < 0 )
        f += FACE_CNT;
	return Facing{ (Facing::Face)f };
}

Facing Facing::operator+( const int bias ) const
{
	return Facing( (Facing::Face)( ( _face + bias ) % FACE_CNT ) );
}

Facing Facing::operator-( const int bias ) const
{
	int f = _face - bias;
	while( f < 0 )
		f += FACE_CNT;
	return Facing{ (Facing::Face)f };
}

Facing& Facing::operator--()
{
    return *this -= 1;
}

Facing Facing::operator--(int)
{
    Facing ret{ _face };
    --( *this );
    return ret;
}

Facing& Facing::operator++()
{
    return *this += 1;
}

Facing Facing::operator++(int)
{
    Facing ret{ _face };
    ++( *this );
    return ret;
}

Facing& Facing::operator+=( const int bias )
{
    _face = (Facing::Face)( ( _face + bias ) % FACE_CNT );
    return *this;
}

Facing& Facing::operator-=( const int bias )
{
    int f{ _face - bias };
    while( f < 0 )
        f += FACE_CNT;
    _face = (Facing::Face)f;
    return *this;
}

Facing Facing::operator<<( const int bias ) const
{
    int f{ _face - bias };
    while( f < 0 )
        f += FACE_CNT;
    return Facing{ (Facing::Face)f };
}

Facing Facing::operator>>( const int bias ) const
{
    return Facing{ (Facing::Face)(( _face + bias ) % FACE_CNT ) };
}

std::ostream& operator << (std::ostream& os, const Facing& f )
{
    os << (char)( 'A' + f.face() );
    return os;
}

Facing::iterator::iterator( const Facing face )
: _face{ new Facing( face ) }
, _cur{ face.face() }
, _cnt( 0 )
{}

Facing::iterator::iterator( const iterator& other )
: _face{other._face}, _cur{ other._cur },_cnt{other._cnt}
{}

bool Facing::iterator::operator==( const iterator& other ) const
{
    return _cur == other._cur;
}

bool Facing::iterator::operator !=(const iterator& other ) const
{
    return _cur != other._cur;
}

Facing::Face Facing::iterator::operator*()
{
    return _cur;
}

Facing::iterator& Facing::iterator::operator++()
{
    if( _cnt < FACE_CNT-1 )
        _cur = (*_face >> ++_cnt).face();
    else
        _cur = FACE_CNT;
    return *this;
}

Facing::iterator Facing::iterator::operator++(int)
{
    iterator old(*this);
    ++(*this);
    return old;
}

Facing::iterator Facing::iterator::begin()
{
    return iterator{ *_face };
}

Facing::iterator Facing::iterator::end()
{
    return iterator{ Facing{ FACE_CNT } };
}

Facing::iterator& Facing::iterator::next()
{
    return ++(*this);
}


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

Hex::Hex( coord_t col, coord_t row)
: _col{col}, _row{row}
{}

Hex::Hex( const Hex& obj )
: _col{ obj._col }, _row{ obj._row }
{}

Hex::Hex( Offset& off )
: _col{ off.dc() }, _row{ off.dr() }
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
        w0.push();
        while( w0.hex() != w1.hex() )
        {
            // w0 pen is still down
            w0.move( m[isOdd(cnt++)], 1 );
            std::cout << "insert " << w0.hex() << std::endl;
        }
        w0.pop();
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

_settings::_settings()
    : oddGrid( false )
    , clippingOn( true )
    {}

_settings::~_settings()
{}

_settings& _settings::instance()
{
    static _settings instance;
    return instance;
}

const int _settings::gridDominance() const
{
    return oddGrid ? 0 : 1;
}

const bool _settings::isOddGrid() const
{
    return oddGrid;
}

bool _settings::setOddGrid( bool val )
{
    std::swap( oddGrid, val );
    return val;
}

const bool _settings::isClippingOn() const
{
    return clippingOn;
}

bool _settings::setClipping( bool val )
{
    std::swap( clippingOn, val );
    return val;
}


_settings & settings = _settings::instance();

} // end ns hexpaper
} // end ns codefool
} // end ns org




