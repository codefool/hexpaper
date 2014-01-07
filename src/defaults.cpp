// hexpaper - library for working with hexagon grids
//
// Copyright (C) 2014 Garyl Hester
//
// defaults.cpp - implementation of the _defaults class
//
//
#include <algorithm>
#include "defaults.hpp"

namespace org {
namespace codefool {
namespace hexpaper {

_defaults::_defaults()
    : oddGrid( false )
    , clippingOn( true )
    {}

_defaults::~_defaults()
{}

_defaults& _defaults::instance()
{
	static _defaults instance;
	return instance;
}

const bool _defaults::isOddGrid() const
{
	return oddGrid;
}

bool _defaults::setOddGrid( bool val )
{
	std::swap( oddGrid, val );
	return val;
}

const bool _defaults::isClippingOn() const
{
	return clippingOn;
}

bool _defaults::setClipping( bool val )
{
	std::swap( clippingOn, val );
	return val;
}


_defaults & defaults = _defaults::instance();

} // end ns hexpaper
} // end ns codefool
} // end ns org




