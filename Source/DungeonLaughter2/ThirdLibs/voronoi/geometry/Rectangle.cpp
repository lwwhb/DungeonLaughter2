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
#include <algorithm>
#include <cmath>

#include "Rectangle.h"

using namespace geometry;

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(const VPoint& a, const VPoint& b)
{
    _left = std::min(a.x(), b.x());
    _width = std::abs(a.x() - b.x());
    _top = std::min(a.y(), b.y());
    _height = std::abs(a.y() - b.y());
}

Rectangle::Rectangle(double left, double top, double width, double height) : _top(top), _left(left),  _width(width), _height(height)
{
}

double Rectangle::x() const
{
    return _left;
}

double Rectangle::y() const
{
    return _top;
}

double Rectangle::width() const
{
    return _width;
}

double Rectangle::height() const
{
    return _height;
}

double Rectangle::left() const
{
    return _left;
}

double Rectangle::right() const
{
    return _left + _width;
}

double Rectangle::top() const
{
    return _top;
}

double Rectangle::bottom() const
{
    return _top + _height;
}

VPoint Rectangle::topLeft() const
{
    return VPoint(top(), left());
}

VPoint Rectangle::topRight() const
{
    return VPoint(top(), right());
}

VPoint Rectangle::bottomLeft() const
{
    return VPoint(bottom(), left());
}

VPoint Rectangle::bottomRight() const
{
    return VPoint(bottom(), right());
}

bool Rectangle::contains(const VPoint& p) const
{
    return left() <= p.x() && p.x() <= right() && top() <= p.y() && p.y() <= bottom();
}

void Rectangle::resizeToInclude(const VPoint& p)
{
    if (contains(p)) {
        return;
    }
    
    if (p.x() < left()) {
        _width += left() - p.x();
        _left = p.x();
    } else if (p.x() > right()) {
        _width += p.x() - right();
    }
    
    if (p.y() < top()) {
        _height += top() - p.y();
        _top = p.y();
    } else if (p.y() > bottom()) {
        _height += p.y() - bottom();
    }
}
