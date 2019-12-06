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

#include "interp.h"

using namespace noise::module;

Select::Select(const ModuleBase& lowModule, const ModuleBase& highModule, const ModuleBase& controlModule)
    : ModuleBase(3)
    , m_edgeFalloff(DEFAULT_SELECT_EDGE_FALLOFF)
    , m_lowerBound(DEFAULT_SELECT_LOWER_BOUND)
    , m_upperBound(DEFAULT_SELECT_UPPER_BOUND)
{
    m_pSourceModule[0] = &lowModule;
    m_pSourceModule[1] = &highModule;
    m_pSourceModule[2] = &controlModule;
}

Select::Select(const ModuleBase& lowModule, const ModuleBase& highModule, const ModuleBase& controlModule, double lowerBound, double upperBound, double edgeFalloff)
    : ModuleBase(3)
    , m_edgeFalloff(edgeFalloff)
    , m_lowerBound(lowerBound)
    , m_upperBound(upperBound)
{
    m_pSourceModule[0] = &lowModule;
    m_pSourceModule[1] = &highModule;
    m_pSourceModule[2] = &controlModule;
}

void Select::setControlModule(const ModuleBase& controlModule)
{
    assert(m_pSourceModule != NULL);
    m_pSourceModule[2] = &controlModule;
}

const ModuleBase& Select::getControlModule() const
{
    if (m_pSourceModule == NULL || m_pSourceModule[2] == NULL)
    {
        throw noise::ExceptionNoModule();
    }
    return *(m_pSourceModule[2]);
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
    assert(m_pSourceModule[0] != NULL);
    assert(m_pSourceModule[1] != NULL);
    assert(m_pSourceModule[2] != NULL);

    double controlValue = m_pSourceModule[2]->getValue(x, y, z);
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
            return LinearInterp(m_pSourceModule[0]->getValue(x, y, z), m_pSourceModule[1]->getValue(x, y, z), alpha);
        }
        else if (controlValue < (m_upperBound - m_edgeFalloff))
        {
            // The output value from the control module is within the selector
            // threshold; return the output value from the second source module.
            return m_pSourceModule[1]->getValue(x, y, z);
        }
        else if (controlValue < (m_upperBound + m_edgeFalloff))
        {
            // The output value from the control module is near the upper end of the
            // selector threshold and within the smooth curve. Interpolate between
            // the output values from the first and second source modules.
            double lowerCurve = (m_upperBound - m_edgeFalloff);
            double upperCurve = (m_upperBound + m_edgeFalloff);
            alpha = SCurve3((controlValue - lowerCurve) / (upperCurve - lowerCurve));
            return LinearInterp(m_pSourceModule[1]->getValue(x, y, z), m_pSourceModule[0]->getValue(x, y, z), alpha);
        }
        else
        {
            // Output value from the control module is above the selector threshold;
            // return the output value from the first source module.
            return m_pSourceModule[0]->getValue(x, y, z);
        }
    }
    else
    {
        if (controlValue < m_lowerBound || controlValue > m_upperBound)
        {
            return m_pSourceModule[0]->getValue(x, y, z);
        }
        else
        {
            return m_pSourceModule[1]->getValue(x, y, z);
        }
    }
}
