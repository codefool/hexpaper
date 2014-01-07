/*
 * datatypes.hpp
 *
 *  Created on: Jan 6, 2014
 *      Author: ghester
 */

#ifndef DATATYPES_HPP_
#define DATATYPES_HPP_

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
enum Facing
{
    FACING_A = 0,
    FACING_B,
    FACING_C,
    FACING_D,
    FACING_E,
    FACING_F,
    FACING_CNT,
    FACING_AB,
    FACING_BC,
    FACING_CD,
    FACING_DE,
    FACING_EF,
    FACING_FA
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
	Offset delta(Facing f) const;
};

} // end ns hexpaper
} // end ns codefool
} // end ns org


#endif /* DATATYPES_HPP_ */
