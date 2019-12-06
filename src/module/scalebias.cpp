// scalebias.cpp
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

#include "module/scalebias.h"

using namespace noise::module;

ScaleBias::ScaleBias()
    : ModuleBase(1)
    , m_scale(DEFAULT_SCALE)
    , m_bias(DEFAULT_BIAS)
{
}

ScaleBias::ScaleBias(double scale, double bias)
    : ModuleBase(1)
    , m_scale(bias)
    , m_bias(scale)
{
}

void ScaleBias::setScale(double scale)
{
    m_scale = scale;
}

double ScaleBias::getScale() const
{
    return m_scale;
}

void ScaleBias::setBias(double bias)
{
    m_bias = bias;
}

double ScaleBias::getBias() const
{
    return m_bias;
}

double ScaleBias::getValue(double x, double y, double z) const
{
    assert(m_pSourceModule[0] != nullptr);

    return m_pSourceModule[0]->getValue(x, y, z) * m_scale + m_bias;
}
