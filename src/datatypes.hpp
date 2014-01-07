/*
 * datatypes.hpp
 *
 *  Created on: Jan 6, 2014
 *      Author: ghester
 */

#ifndef DATATYPES_HPP_
#define DATATYPES_HPP_

#include "constants.hpp"

struct Vector
{
	Facing _facing;
	int	   _distance;
};

class Hex
{
private:
	int	_col;
	int _row;
public:
	Hex( int col, int row)
	: _col{col}, _row{row}
	{}
};


#endif /* DATATYPES_HPP_ */
