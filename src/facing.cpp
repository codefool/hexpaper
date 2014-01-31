/*
 * facing.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: ghester
 */

#include "hexpaper"

namespace org {
namespace codefool {
namespace hexpaper {

const Facing _FacingA{ Facing::FACE_A };
const Facing _FacingB{ Facing::FACE_B };
const Facing _FacingC{ Facing::FACE_C };
const Facing _FacingD{ Facing::FACE_D };
const Facing _FacingE{ Facing::FACE_E };
const Facing _FacingF{ Facing::FACE_F };
const Facing _FacingX{ Facing::FACE_CNT };

Facing::Facing( const Face f, const Face f2 )
: _face(f), _face2(f2)
{}

Facing::Facing( const char face )
: _face{ FACE_A }, _face2( FACE_CNT )
{
    char c{(char)::tolower(face)};
    if( 'a' <= c && c <= 'f' )
        _face = (Face)(c - 'a');
}

Facing::Facing( const Facing& obj )
: _face{ obj._face }, _face2{ obj._face2 }
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

Facing& Facing::setDoubleFace( const Face f2 )
{
    _face2 = f2;
    return *this;
}

bool Facing::isDouble() const
{
    return _face2 != FACE_CNT;
}

std::ostream& operator << (std::ostream& os, const Facing& f )
{
    os << (char)( 'A' + f.face() );
    if( f.isDouble() )
        os << (char)( 'A' + f.face2() );
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

} // end ns hexpaper
} // end ns codefool
} // end ns org


