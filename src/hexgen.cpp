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
                    os << "< " << std::setw(2) << std::setfill('0') << iCol
                       <<         std::setw(2) << std::setfill('0') << iRow;
                    if( found )
                        os << '*';
                    else
                        os << ' ';
                    os << ">--";
                }
                else
                {
                    if( iCol == 2 )
                        os << "  ";
                    os << ">--< " << std::setw(2) << std::setfill('0') << iCol
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
