/*
 * hexgen.cpp
 *
 *  Created on: Jan 12, 2014
 *      Author: ghester
 */
// Distributed under GNU LESSER GENERAL PUBLIC LICENSE v2.1 (http://www.gnu.org/licenses/lgpl-2.1.html)
//
// This project lives at http://github.com/codefool/hexpaper
//
// Copyright (C) 2014 Garyl Hester
//
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include "hexpaper"

namespace org {
namespace codefool {
namespace hexpaper {

// generate a text hexgrid to os based on the given HexInfo structure
//
// Odd grid, odd origin: (1^1=0)
//
//....:....1....:....2....:....3....:....4....:....5....:
//  +--+      +--+      +--+      +--+      +--+         */
// /    \    /    \    /    \    /    \    /    \        */
//+ 0101 +--+ 0301 +--+ 0501 +--+ 0701 +--+ 0901 +--+    */
// \    /    \    /    \    /    \    /    \    /    \   */
//  +--+ 0201 +--+ 0401 +--+ 0601 +--+ 0801 +--+ 1001 +  */
// /    \    /    \    /    \    /    \    /    \    /   */
//+ 0102 +--+ 0302 +--+ 0502 +--+ 0702 +--+ 0902 +--+    */
// \    /    \    /    \    /    \    /    \    /    \   */
//
// Odd grid, even origin: (1^0=1)
//
//....:....1....:....2....:....3....:....4....:....5....:
//       +--+      +--+      +--+      +--+      +--+    */
//      /    \    /    \    /    \    /    \    /    \   */
//  +--+ 0301 +--+ 0501 +--+ 0701 +--+ 0901 +--+ 1101 +  */
// /    \    /    \    /    \    /    \    /    \    /   */
//+ 0201 +--+ 0401 +--+ 0601 +--+ 0801 +--+ 1001 +--+    */
// \    /    \    /    \    /    \    /    \    /    \   */
//  +--+ 0302 +--+ 0502 +--+ 0702 +--+ 0902 +--+ 1102 +  */
// /    \    /    \    /    \    /    \    /    \    /   */
//
//
// Even grid, odd origin: (0^1=1)
//....:....1....:....2....:....3....:....4....:....5....:
//       +--+      +--+      +--+      +--+      +--+    */
//      /    \    /    \    /    \    /    \    /    \   */
//  +--+ 0201 +--+ 0401 +--+ 0601 +--+ 0801 +--+ 1001 +  */
// /    \    /    \    /    \    /    \    /    \    /   */
//+ 0101 +--+ 0301 +--+ 0501 +--+ 0701 +--+ 0901 +--+    */
// \    /    \    /    \    /    \    /    \    /    \   */
//  +--+ 0202 +--+ 0402 +--+ 0602 +--+ 0802 +--+ 1002 +  */
// /    \    /    \    /    \    /    \    /    \    /   */
//
// Even grid, even origin: (0^0=0)
//
//....:....1....:....2....:....3....:....4....:....5....:
//  +--+      +--+      +--+      +--+      +--+         */
// /    \    /    \    /    \    /    \    /    \        */
//+ 0201 +--+ 0401 +--+ 0601 +--+ 0801 +--+ 1001 +--+    */
// \    /    \    /    \    /    \    /    \    /    \   */
//  +--+ 0301 +--+ 0501 +--+ 0701 +--+ 0901 +--+ 1101 +  */
// /    \    /    \    /    \    /    \    /    \    /   */
//+ 0202 +--+ 0402 +--+ 0602 +--+ 0802 +--+ 1002 +--+    */
// \    /    \    /    \    /    \    /    \    /    \   */
//
// So the template we produce is guided by isOdd(grid) ^ isOdd(ulc.col)

std::ostream& operator << ( std::ostream& os, const HexInfo& hi )
{
    for( coord_t iRow = hi._ulc.col(); iRow <= hi._lrc.col(); ++iRow )
    {
        for( int iOdd = 0; iOdd < 2; ++iOdd )
        {
            for( coord_t iCol = hi._ulc.row() + iOdd; iCol <= hi._lrc.row() + iOdd; iCol+=2)
            {
                if( !iOdd )
                    os << " /    \\   ";
                else
                    os << " \\    /   ";
            }
            os << std::endl;

            for( coord_t iCol = hi._ulc.row() + iOdd; iCol <= hi._lrc.row() + iOdd; iCol += 2)
            {
                bool found{ nullptr != hi._hexs && hi._hexs->end() != std::find( hi._hexs->begin(), hi._hexs->end(), Hex{iCol,iRow} ) };

                if( !iOdd )
                {
                    os << "+ " << std::setw(2) << std::setfill('0') << iCol
                       <<         std::setw(2) << std::setfill('0') << iRow
                       << ((found)?'*':' ')
                       << "+--";
                }
                else
                {
                    if( iCol == 2 )
                        os << "  ";
                    os << "+--+ " << std::setw(2) << std::setfill('0') << iCol
                       <<            std::setw(2) << std::setfill('0') << iRow
                       << ((found)?'*':' ');
                }
            }
            os << std::endl;
        }
    }
	return os;
}

} // end ns hexpaper
} // end ns codefool
} // end ns org
