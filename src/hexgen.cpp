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
    // generate the header rows
    int numCols{ hi._lrc.col() - hi._ulc.col() + 1 };
    int numRows{ hi._lrc.row() - hi._ulc.row() + 1 };
    bool isEvenImage{ (bool)(settings.isOddGrid() ^ isOdd( hi._ulc.col() )) };

    std::string uslash("");
    //std::string lslash("");
    if( isEvenImage )
    {
        os << "     ";
        uslash += "     ";
        //lslash += "     ";
    }
    for( int col = isEvenImage; col < numCols; col += 2 )
    {
        os << "  +--+    ";
        uslash +=  " /    \\   ";
        //lslash +=  " \\    /   ";
    }
    std::cout << std::endl << uslash << std::endl;

    std::stringstream inslash;
    std::stringstream outslash;
    inslash  << " /    ";
    outslash << " \\    ";
    for( int col = 0; col < numCols; col++ )
    {
        if( isOdd( col ) )
        {
            inslash  << "/    ";
            outslash << "\\    ";
        }
        else
        {
            inslash  << "\\    ";
            outslash << "/    ";
        }
    }

    for( int row = 0; row < numRows; ++row )
    {
        // build the two halves of the hexline at the same time
        // the order in which we print these is dependent on
        // isEvenImage.
        std::stringstream inset;
        std::stringstream outset;
        // build the inset line
        //  +--+ 0201 +--+ 0401 +--+ 0601 +--+ 0801 +--+ 1001 +  */
        // /    \    /    \    /    \    /    \    /    \    /   */
        inset << "  +--+ ";
        for( int col = 1; col < numCols; col+=2 )
        {
            bool found{ nullptr != hi._hexs && hi._hexs->end() != std::find( hi._hexs->begin(), hi._hexs->end(), Hex{hi._ulc.col()+col,hi._ulc.row()+row} ) };
            inset << std::setw(2) << std::setfill('0') << ( hi._ulc.col() + col )
                  << std::setw(2) << std::setfill('0') << ( hi._ulc.row() + row )
                  << ((found)?'*':' ') << '+';
            if( hi._ulc.col() + col < hi._lrc.col() )
                inset << "--+ ";
        }

        // build the outset line
        //+ 0101 +--+ 0301 +--+ 0501 +--+ 0701 +--+ 0901 +--+    */
        // \    /    \    /    \    /    \    /    \    /    \   */
        outset << "+";
        for( int col = 0; col < numCols; col+=2 )
        {
            bool found{ nullptr != hi._hexs && hi._hexs->end() != std::find( hi._hexs->begin(), hi._hexs->end(), Hex{hi._ulc.col()+col,hi._ulc.row()+row} ) };
            outset << ' ' << std::setw(2) << std::setfill('0') << ( hi._ulc.col() + col )
                   << std::setw(2) << std::setfill('0') << ( hi._ulc.row() + row )
                   << ((found)?'*':' ') << '+';
            if( hi._ulc.col() + col < hi._lrc.col() )
                   outset << "--+";
        }

        if( isEvenImage )
        {
            std::cout << inset.str() << std::endl
                      << inslash.str() << std::endl
                      << outset.str() << std::endl
                      << outslash.str() << std::endl;
        }
        else
        {
            std::cout << outset.str() << std::endl
                      << outslash.str() << std::endl
                      << inset.str() << std::endl
                      << inslash.str() << std::endl;
        }
    }
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
                bool found{ nullptr != hi._hexs && hi._hexs->end() != std::find( hi._hexs->begin(), hi._hexs->end(), Hex{iCol,iRow} ) };

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
