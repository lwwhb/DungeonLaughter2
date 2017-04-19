/**
 * (C) LGPL-3
 *
 * Voronoi++ <https://github.com/rlux/voronoi>
 *
 * Copyright: 2012 Lux, Scheibel
 * Authors:
 *     Roland Lux <rollux2000@googlemail.com>
 *     Willy Scheibel <willyscheibel@gmx.de>
 *
 * This file is part of Voronoi++.
 *
 * Voronoi++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Voronoi++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Voronoi++.  If not, see <http://www.gnu.org/licenses/>.
 **/
#include "DungeonLaughter2.h"
#include "Circle.h"
#include "Line.h"

using namespace geometry;

Circle::Circle() : _center(VPoint()), _radius(0)
{
}

Circle::Circle(const VPoint& center, double radius) : _center(center), _radius(radius)
{
}

Circle::Circle(const VPoint& a, const VPoint& b, const VPoint& c)
{
    VVector ab = b-a;
    VVector bc = c-b;
    
    if (ab.x()*bc.y()-ab.y()*bc.x()==0) { // all VPoints on a line
        return;
    }
    
    LineIntersectionSolutionSet solutionSet = Line::forNormal(a.midPoint(b), ab).intersection(Line::forNormal(b.midPoint(c),bc));
    if (!solutionSet.isOne()) {
        return;
    }
    
    _center = solutionSet.point();
    _radius = (_center-a).length();
}

const VPoint& Circle::center() const
{
    return _center;
}

double Circle::radius() const
{
    return _radius;
}

bool Circle::contains(const VPoint& p) const
{
    return (_center-p).squaredLength()<=_radius*_radius;
}

VPoint Circle::top() const
{
    return VPoint(_center.x(), _center.y() - _radius);
}

VPoint Circle::bottom() const
{
    return VPoint(_center.x(), _center.y() + _radius);
}

VPoint Circle::left() const
{
    return VPoint(_center.x() - _radius, _center.y());
}

VPoint Circle::right() const
{
    return VPoint(_center.x() + _radius, _center.y());
}
