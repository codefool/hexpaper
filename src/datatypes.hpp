/*
 * datatypes.hpp
 *
 *  Created on: Jan 6, 2014
 *      Author: ghester
 */

#ifndef DATATYPES_HPP_
#define DATATYPES_HPP_

#include <iterator>

namespace org {
namespace codefool {
namespace hexpaper {

typedef signed char coord_t;

//  Primary Facings:
//       ___
//   ___/ A \___
//  / F \___/ B \  *
//  \___/   \___/
//  / E \___/ C \  *
//  \___/ D \___/
//      \___/
//
class Facing
{
public:
    enum Face
    {
        FACING_A = 0,
        FACING_B,
        FACING_C,
        FACING_D,
        FACING_E,
        FACING_F,
        FACING_CNT
    };

    Facing( Face f )
    : _face(f)
    {}

    Face face( void ) const { return _face; }

private:
    Face    _face;

public:
    class iterator : public std::iterator<std::bidirectional_iterator_tag, Face>
    {
    public:
        iterator( const Face face = FACING_A )
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

        iterator operator++()
        {
            if( _cnt < FACING_CNT )
            {
                _cur = (Face)(( _face + ++_cnt ) % FACING_CNT );
            }
            return *this;
        }

        iterator operator++(int)
        {
            iterator old(*this);
            ++(*this);
            return old;
        }

        iterator operator--()
        {
            if( _cnt > 0 )
            {
                _cur = (Face)(( _face + --_cnt ) % FACING_CNT );
            }
            return *this;
        }

        iterator operator--(int)
        {
            iterator old(*this);
            --(*this);
            return old;
        }

        iterator begin()
        {
            return iterator( _face );
        }

        iterator end()
        {
            return iterator( (Face)-1 );
        }

    private:
        Face _face;
        Face _cur;
        int  _cnt;
    };
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
private:
    coord_t _dc;
    coord_t _dr;
public:
    Offset(coord_t dc, coord_t dr);
    Offset(const Offset& obj );
    ~Offset();
    inline coord_t dc( void ) const { return _dc; }
    inline coord_t dr( void ) const { return _dr; }
};

class Hex
{
private:
    coord_t	_col;
    coord_t _row;
public:
	Hex( coord_t col, coord_t row);
	Hex( Offset& off );
	~Hex();
	Offset delta(Facing::Face f) const;
};

} // end ns hexpaper
} // end ns codefool
} // end ns org


#endif /* DATATYPES_HPP_ */
