/*
 * constants.hpp
 *
 *  Created on: Jan 6, 2014
 *      Author: ghester
 */
#include "datatypes.hpp"

#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

namespace org {
namespace codefool {
namespace hexpaper {

// Grid Orientation
//
// An odd-grid is on where the hex of an odd-column is above (at facing B or F) of the
// same row number. An even-grid is the opposite:
//
//  Odd Grid:          Even Grid:             Primary Facings:
//   __    __    __        __    __                ___
//  /11\__/31\__/51\    __/21\__/41\__         ___/ A \___
//  \__/21\__/41\__/   /11\__/31\__/51\       / F \___/ B \   *
//  /12\__/32\__/52\   \__/22\__/42\__/       \___/   \___/
//  \__/22\__/42\__/   /12\__/32\__/52\       / E \___/ C \   *
//  /13\__/33\__/53\   \__/23\__/43\__/       \___/ D \___/
//  \__/23\__/43\__/   /13\__/33\__/53\           \___/
//  /14\__/34\__/54\   \__/24\__/44\__/
//
// Delta Column (dc) and Delta row (dr) for each facing, for odd grid:
//
//                 A   B   C   D   E   F
// dc              0   1   1   0  -1  -1
// dr (odd col)   -1  -1   0   1   0  -1
// dr (even col)  -1   0   1   1   1   0
//
// And for even grid:
//
//                 A   B   C   D   E   F
// dc              0   1   1   0  -1  -1
// dr (odd col)   -1   0   1   1   1   0
// dr (even col)  -1  -1   0   1   0  -1
//
// Note that the dr values for odd/even col for odd grid are even/odd col for even grid.
// Hence, we can determine which we should use by IS_ODD(col) XOR IS_ODD(grid)
//
extern coord_t _dc[6];
extern coord_t _dr[][6];

#define IS_ODD(x) (((x)&0x1)==0)
#define IS_EVEN(x) (!IS_ODD(x))

} // end ns hexpaper
} // end ns codefool
} // end ns org

#endif /* CONSTANTS_HPP_ */
