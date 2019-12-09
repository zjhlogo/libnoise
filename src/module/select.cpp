// select.cpp
//
// Copyright (C) 2003, 2004 Jason Bevins
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//

#include "module/select.h"
#include "scalarparameter.h"

#include "interp.h"

using namespace noise::module;

Select::Select(const noise::ScalarParameter& low, const noise::ScalarParameter& high, const noise::ScalarParameter& control)
    : m_edgeFalloff(DEFAULT_SELECT_EDGE_FALLOFF)
    , m_lowerBound(DEFAULT_SELECT_LOWER_BOUND)
    , m_upperBound(DEFAULT_SELECT_UPPER_BOUND)
    , m_low(low)
    , m_high(high)
    , m_control(control)
{
}

Select::Select(const noise::ScalarParameter& low, const noise::ScalarParameter& high, const noise::ScalarParameter& control, double lowerBound, double upperBound, double edgeFalloff)
    : m_edgeFalloff(edgeFalloff)
    , m_lowerBound(lowerBound)
    , m_upperBound(upperBound)
    , m_low(low)
    , m_high(high)
    , m_control(control)
{
}

void Select::setLowModule(const noise::ScalarParameter& low)
{
    m_low = low;
}

const noise::ScalarParameter& Select::getLowModule() const
{
    return m_low;
}

void Select::setHighModule(const noise::ScalarParameter& high)
{
    m_high = high;
}

const noise::ScalarParameter& Select::getHighModule() const
{
    return m_high;
}

void Select::setControlModule(const noise::ScalarParameter& control)
{
    m_control = control;
}

const noise::ScalarParameter& Select::getControlModule() const
{
    return m_control;
}

void Select::setBounds(double lowerBound, double upperBound)
{
    assert(lowerBound <= upperBound);

    m_lowerBound = lowerBound;
    m_upperBound = upperBound;

    // Make sure that the edge falloff curves do not overlap.
    setEdgeFalloff(m_edgeFalloff);
}

double Select::getLowerBound() const
{
    return m_lowerBound;
}

double Select::getUpperBound() const
{
    return m_upperBound;
}

void Select::setEdgeFalloff(double edgeFalloff)
{
    // Make sure that the edge falloff curves do not overlap.
    double boundSize = m_upperBound - m_lowerBound;
    m_edgeFalloff = (edgeFalloff > boundSize / 2) ? boundSize / 2 : edgeFalloff;
}

double Select::getEdgeFalloff() const
{
    return m_edgeFalloff;
}

double Select::getValue(double x, double y, double z) const
{
    double controlValue = m_control.getValue(x, y, z);
    double alpha;
    if (m_edgeFalloff > 0.0)
    {
        if (controlValue < (m_lowerBound - m_edgeFalloff))
        {
            // The output value from the control module is below the selector
            // threshold; return the output value from the first source module.
            return m_pSourceModule[0]->getValue(x, y, z);
        }
        else if (controlValue < (m_lowerBound + m_edgeFalloff))
        {
            // The output value from the control module is near the lower end of the
            // selector threshold and within the smooth curve. Interpolate between
            // the output values from the first and second source modules.
            double lowerCurve = (m_lowerBound - m_edgeFalloff);
            double upperCurve = (m_lowerBound + m_edgeFalloff);
            alpha = SCurve3((controlValue - lowerCurve) / (upperCurve - lowerCurve));
            return LinearInterp(m_low.getValue(x, y, z), m_high.getValue(x, y, z), alpha);
        }
        else if (controlValue < (m_upperBound - m_edgeFalloff))
        {
            // The output value from the control module is within the selector
            // threshold; return the output value from the second source module.
            return m_high.getValue(x, y, z);
        }
        else if (controlValue < (m_upperBound + m_edgeFalloff))
        {
            // The output value from the control module is near the upper end of the
            // selector threshold and within the smooth curve. Interpolate between
            // the output values from the first and second source modules.
            double lowerCurve = (m_upperBound - m_edgeFalloff);
            double upperCurve = (m_upperBound + m_edgeFalloff);
            alpha = SCurve3((controlValue - lowerCurve) / (upperCurve - lowerCurve));
            return LinearInterp(m_high.getValue(x, y, z), m_low.getValue(x, y, z), alpha);
        }
        else
        {
            // Output value from the control module is above the selector threshold;
            // return the output value from the first source module.
            return m_low.getValue(x, y, z);
        }
    }
    else
    {
        if (controlValue < m_lowerBound || controlValue > m_upperBound)
        {
            return m_low.getValue(x, y, z);
        }
        else
        {
            return m_high.getValue(x, y, z);
        }
    }
}
