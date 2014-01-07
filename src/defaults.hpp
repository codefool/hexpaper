/*
 * defaults.hpp
 *
 *  Created on: Jan 6, 2014
 *      Author: ghester
 */

#ifndef DEFAULTS_HPP_
#define DEFAULTS_HPP_

namespace org {
namespace codefool {
namespace hexpaper {

class _defaults
{
private:
    bool oddGrid;   	// true if grid is odd-oriented, false if even
    bool clippingOn;	// true if clipping is on

    _defaults();
    ~_defaults();

public:
    static _defaults& instance();
    const bool isOddGrid() const;
    bool setOddGrid(bool val);
    const bool isClippingOn() const;
    bool setClipping(bool val);
};

extern _defaults& defaults;		// forward decl

} // end ns hexpaper
} // end ns codefool
} // end ns org




#endif /* DEFAULTS_HPP_ */
