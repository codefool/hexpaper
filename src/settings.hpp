/*
 * defaults.hpp
 *
 *  Created on: Jan 6, 2014
 *      Author: ghester
 */

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

namespace org {
namespace codefool {
namespace hexpaper {

class _settings
{
private:
    bool oddGrid;   	// true if grid is odd-oriented, false if even
    bool clippingOn;	// true if clipping is on

    _settings();
    ~_settings();

public:
    static _settings& instance();
    const signed char gridOrientation() const;
    const bool isOddGrid() const;
    bool setOddGrid(bool val);
    const bool isClippingOn() const;
    bool setClipping(bool val);
};

extern _settings& settings;		// forward decl

} // end ns hexpaper
} // end ns codefool
} // end ns org




#endif /* SETTINGS_HPP_ */
