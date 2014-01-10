/*
 * datatypes.hpp
 *
 *  Created on: Jan 6, 2014
 *      Author: ghester
 */

#ifndef DATATYPES_HPP_
#define DATATYPES_HPP_

#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <deque>
#include <map>
#include <array>

namespace org {
namespace codefool {
namespace hexpaper {

typedef signed int coord_t;

//  Primary Facings:
//       ___
//   ___/ A \___
//  / F \___/ B \  *
//  \___/   \___/
//  / E \___/ C \  *
//  \___/ D \___/
//      \___/
//
class Facing;

extern const Facing _FacingA;
extern const Facing _FacingB;
extern const Facing _FacingC;
extern const Facing _FacingD;
extern const Facing _FacingE;
extern const Facing _FacingF;

class Facing
{
public:
    enum Face
    {
        FACE_A = 0,
        FACE_B,
        FACE_C,
        FACE_D,
        FACE_E,
        FACE_F,
        FACE_CNT
    };

    Facing( const Face f = FACE_A );
    Facing operator-( const Facing& rhs ) const;
    Facing operator+( const Facing& rhs ) const ;
    Facing& operator--();
    Facing  operator--(int);
    Facing& operator++();
    Facing  operator++(int);
    Facing& operator+=( const int bias );
    Facing& operator-=( const int bias );
    Facing operator<<( const int bias ) const;
    Facing operator>>( const int bias ) const;
    Face face( void ) const { return _face; }
    operator int() const { return (int)_face; }
    friend std::ostream& operator << (std::ostream& os, const Facing& f );

    class iterator : public std::iterator<std::forward_iterator_tag, Face>
    {
    public:
        iterator( const Face face = FACE_A )
        : _face{ face }, _cur{ face }, _cnt{ 0 }
        {}

        iterator( const iterator& other )
        : _face{other._face}, _cur{ other._cur },_cnt{other._cnt}
        {}

        bool operator==( const iterator& other ) const
        {
            return _cur == other._cur;
        }

        bool operator !=(const iterator& other ) const
        {
            return _cur != other._cur;
        }

        Face operator*()
        {
            return _cur;
        }

        iterator& operator++()
        {
            if( _cnt < FACE_CNT-1 )
            {
                _cur = (Face)(( _face + ++_cnt ) % FACE_CNT );
            }
            else
                _cur = FACE_CNT;
            return *this;
        }

        iterator operator++(int)
        {
            iterator old(*this);
            ++(*this);
            return old;
        }

        iterator begin()
        {
            return iterator( _face );
        }

        iterator end()
        {
            return iterator( FACE_CNT );
        }

        iterator& next()
        {
            return ++(*this);
        }


    private:
        Face _face;
        Face _cur;
        int  _cnt;
    };

    iterator begin() { return Facing::iterator( _face ); }
    iterator end()   { return Facing::iterator( FACE_CNT ); }

private:
    Face    _face;

public:
};

enum Quadrant
{
    QUAD_0,
    QUAD_1,
    QUAD_2,
    QUAD_3,
    QUAD_CNT
};

struct Vector
{
	Facing _facing;
	int	   _range;
};

class Offset
{
public:
    Offset(coord_t dc, coord_t dr);
    Offset(const Offset& obj );
    ~Offset();
    inline coord_t dc( void ) const { return _dc; }
    inline coord_t dr( void ) const { return _dr; }
private:
    coord_t _dc;
    coord_t _dr;
};

class Hex
{
public:
	Hex( coord_t col, coord_t row);
	Hex( Offset& off );
	virtual ~Hex();
	coord_t row( void ) const { return _row; }
	coord_t col( void ) const { return _col; }
	Offset delta(Facing f) const;
	Hex& move( Facing dir, int distance = 1, Facing bias = _FacingA );
	Hex& operator+( const Offset& off );
	bool operator==(const Hex& rhs ) const;
	bool operator!=(const Hex& rhs ) const;
	friend std::ostream& operator<<(std::ostream& os, const Hex& hex);
private:
    coord_t _col;
    coord_t _row;
};

class HexWalker
{
public:
    HexWalker( const Hex& hex, const bool noDups = true );
    HexWalker( const HexWalker& obj );

    void setOrigin( const Hex& hex );
    // walk the hex around
    //     n   - list of digits - repeat the following command n times.
    //     A-F - move pen in that direction relative to face.
    //     P   - pen down (start recording all hex's from current hex on.)
    //     U   - pen up   (stop recording hex's.)
    //     M   - mark - push this hex on the stack.
    //     R   - recall - pop hex off the stack (and make that hex the current hex.)
    void walk( std::string&& path, Facing bias = _FacingA );
    void penUp( void );
    void penDown( void );
    void push( void );
    void pop( void );
    void move( Facing dir, int cnt, Facing bias = _FacingA );
    void sort( void );
    const std::vector<Hex>& trail( void ) const;
    const Hex& hex( void ) const;

private:
    Hex              _h;
    std::vector<Hex> _trail;
    std::deque<Hex>  _stack;
    bool             _penDown;
    bool             _noDups;
};

} // end ns hexpaper
} // end ns codefool
} // end ns org


#endif /* DATATYPES_HPP_ */
