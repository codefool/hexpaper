/*
 * hexgen.cpp
 *
 *  Created on: Jan 12, 2014
 *      Author: ghester
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "hexpaper"

namespace org {
namespace codefool {
namespace hexpaper {

// generate a text hexgrid to os based on the given HexInfo structure
//
// Odd grid:
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
//
// Even grid:
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

std::ostream& operator << ( std::ostream& os, const HexInfo& hi )
{
    // generate the header rows
    std::string uslash("");
    std::string lslash("");
    if( isOdd( hi._ulc.col() ) ^ settings.isOddGrid() )
    {
        os << "      ";
        uslash += "      ";
        lslash += "      ";
    }
    for( coord_t iCol = hi._ulc.col(); iCol <= hi._lrc.col(); iCol += 2 )
    {
        os << "  +--+    ";
        uslash +=  " /    \\   ";
        lslash +=  " \\    /   ";
    }
    os << std::endl << uslash << std::endl << lslash << std::endl;

    os << std::endl << std::endl;

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
                bool found{ hi._hexs->end() != std::find( hi._hexs->begin(), hi._hexs->end(), Hex{iCol,iRow} ) };

                if( !iOdd )
                {
                    os << "+ " << std::setw(2) << std::setfill('0') << iCol
                       <<         std::setw(2) << std::setfill('0') << iRow;
                    if( found )
                        os << '*';
                    else
                        os << ' ';
                    os << "+--";
                }
                else
                {
                    if( iCol == 2 )
                        os << "  ";
                    os << "+--+ " << std::setw(2) << std::setfill('0') << iCol
                    <<               std::setw(2) << std::setfill('0') << iRow;
                    if( found )
                        os << '*';
                    else
                        os << ' ';
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
