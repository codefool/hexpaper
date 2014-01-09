// hexpaper - library for working with hexagon grids
//
// Copyright (C) 2014 Garyl Hester
//
// defaults.cpp - implementation of the _defaults class
//
//
#include <algorithm>
#include "settings.hpp"

namespace org {
namespace codefool {
namespace hexpaper {

_settings::_settings()
    : oddGrid( false )
    , clippingOn( true )
    {}

_settings::~_settings()
{}

_settings& _settings::instance()
{
	static _settings instance;
	return instance;
}

const signed char _settings::gridOrientation() const
{
    return oddGrid ? 0 : 1;
}

const bool _settings::isOddGrid() const
{
	return oddGrid;
}

bool _settings::setOddGrid( bool val )
{
	std::swap( oddGrid, val );
	return val;
}

const bool _settings::isClippingOn() const
{
	return clippingOn;
}

bool _settings::setClipping( bool val )
{
	std::swap( clippingOn, val );
	return val;
}


_settings & settings = _settings::instance();

} // end ns hexpaper
} // end ns codefool
} // end ns org




