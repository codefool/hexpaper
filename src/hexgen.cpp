/*
 * hexgen.cpp
 *
 *  Created on: Jan 12, 2014
 *      Author: ghester
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include "hexpaper"

namespace org {
namespace codefool {
namespace hexpaper {

// generate a text hexgrid to os based on the given HexInfo structure

std::ostream& operator << ( std::ostream& os, const HexInfo& hi )
{
    for( coord_t iCol = hi._ulc.col(); iCol <= hi._lrc.col(); ++iCol )
    {
        for( int iOdd = 0; iOdd < 2; ++iOdd )
        {
            for( coord_t iRow = hi._ulc.row() + iOdd; iRow <= hi._lrc.row() + iOdd; iRow+=2)
            {
                if( !iOdd )
                    os << " /    \\   ";
                else
                {
                    os << " \\    /   ";
                }
            }
            os << std::endl;

            for( coord_t iRow = hi._ulc.row() + iOdd; iRow <= hi._lrc.row() + iOdd; iRow += 2)
            {
                if( !iOdd )
                    os << "< " << std::setw(2) << std::setfill('0') << iRow
                       <<         std::setw(2) << std::setfill('0') << iCol << " >--";
                else
                {
                    if( iRow == 2 )
                        os << "  ";
                    os << ">--< " << std::setw(2) << std::setfill('0') << iRow
                    <<               std::setw(2) << std::setfill('0') << iCol
                    << ' ';
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
