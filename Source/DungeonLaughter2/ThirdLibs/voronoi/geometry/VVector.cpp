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
#include <cmath>

#include "VVector.h"
#include "VPoint.h"

using namespace geometry;

VVector geometry::operator*(double factor, const VVector& v) {
    return v * factor;
}

VVector::VVector() : _x(0), _y(0)
{
}

VVector::VVector(double x, double y) : _x(x), _y(y)
{
}

VVector VVector::operator-() const {
    return VVector(-_x, -_y);
}

VVector& VVector::operator+=(const VVector& v) {
    _x += v._x;
    _y += v._y;
    
    return *this;
}

VVector& VVector::operator-=(const VVector& v) {
    _x -= v._x;
    _y -= v._y;
    
    return *this;
}

VVector& VVector::operator*=(double factor) {
    _x *= factor;
    _y *= factor;
    
    return *this;
}

VVector& VVector::operator/=(double factor) {
    _x /= factor;
    _y /= factor;
    
    return *this;
}

VVector VVector::operator+(const VVector& v) const {
    return VVector(_x + v._x, _y + v._y);
}

VVector VVector::operator-(const VVector& v) const {
    return VVector(_x - v._x, _y - v._y);
}

VVector VVector::operator*(double factor) const {
    return VVector(_x * factor, _y * factor);
}

VVector VVector::operator/(double factor) const {
    return VVector(_x / factor, _y / factor);
}

bool VVector::operator==(const VVector& v) const
{
    return _x == v._x && _y == v._y;
}

bool VVector::operator!=(const VVector& v) const
{
    return _x != v._x || _y != v._y;
}

double VVector::dotProduct(const VVector& v) const {
    return _x * v._x + _y * v._y;
}

double VVector::length() const {
    return std::sqrt(squaredLength());
}

double VVector::squaredLength() const {
    return _x * _x + _y * _y;
}

VVector VVector::perpendicular() const
{
    return VVector(-_y, _x);
}

bool VVector::isParallelTo(const VVector& v) const
{
    return dotProduct(v) == 0;
}

double VVector::x() const
{
    return _x;
}

double VVector::y() const
{
    return _y;
}

VPoint VVector::toPoint() const
{
    return VPoint(_x, _y);
}
