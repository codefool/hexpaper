/*
 * datatypes.cpp
 *
 *  Created on: Jan 7, 2014
 *      Author: hesterg
 */
#include "datatypes.hpp"
#include "constants.hpp"
#include "defaults.hpp"

namespace org {
namespace codefool {
namespace hexpaper {

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
Offset Hex::delta( Facing f ) const
{
    coord_t dc = _dc[f];
    coord_t dr = _dr[ IS_ODD( dc ) ^ defaults.gridOrientation() ][f];
    return Offset(dc,dr);
}

} // end ns hexpaper
} // end ns codefool
} // end ns org
