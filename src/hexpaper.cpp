#include "hexpaper"

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
    { -1,  0, 1, 1, 1,  0 },
    { -2, -1, 1, 2, 1, -1 }
};

template <>
int isOdd<char *>(char *c)
{ return isOdd<int>(atoi(c)); }

template <>
int isOdd<std::string>(std::string s)
{ return isOdd<int>(atoi(s.c_str())); }

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


Hex::Hex( coord_t col, coord_t row)
: _col{col}, _row{row}
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
    coord_t dr = _dr[ isOdd( _col ) ^ settings.gridOrientation() ][f];
    return Offset(dc,dr);
}

Hex& Hex::move( const Facing& dir, int distance, const Facing& bias )
{
    while( distance-- )
    {
        Offset off = delta( dir << bias );
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
    // FIXME: This is dependent on grid orientation
    return _col < rhs._col || _row < rhs._row;
}

bool Hex::operator< (const Hex& rhs ) const
{
    // FIXME: This is dependent on grid orientation
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

void HexWalker::setOrigin( const Hex& hex )
{
    _h = hex;
    penUp();
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
            Facing f{(Facing::Face)( c - 'a')};
            move( f + bias, cnt );
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

HexWalker& HexWalker::move( const Facing& dir, int cnt, const Facing& bias )
{
    //if( !cnt ) cnt=1;
    while( cnt-- )
    {
        _h.move(dir,1,bias);
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
    Facing l[2] = { dir, dir - 1 };
    Facing r[2] = { dir, dir + 1 };
    Facing m[2] = { dir + 2, dir + 1 };

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

const signed char _settings::gridOrientation() const
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




