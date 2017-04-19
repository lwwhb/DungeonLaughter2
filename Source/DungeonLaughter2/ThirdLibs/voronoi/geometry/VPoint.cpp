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
#include "VPoint.h"

using namespace geometry;

VPoint geometry::operator*(double factor, const VPoint& p) {
    return p * factor;
}

VPoint::VPoint() : _x(0), _y(0)
{
}

VPoint::VPoint(double x, double y) : _x(x), _y(y)
{
}

VPoint VPoint::operator-() const {
    return VPoint(-_x, -_y);
}

VPoint& VPoint::operator+=(const VVector& v) {
    _x += v.x();
    _y += v.y();
    
    return *this;
}

VPoint& VPoint::operator-=(const VVector& v) {
    _x -= v.x();
    _y -= v.y();
    
    return *this;
}

VPoint& VPoint::operator*=(double factor) {
    _x *= factor;
    _y *= factor;
    
    return *this;
}

VPoint& VPoint::operator/=(double factor) {
    _x /= factor;
    _y /= factor;
    
    return *this;
}

VPoint VPoint::operator+(const VVector& v) const {
    return VPoint(_x + v.x(), _y + v.y());
}

VPoint VPoint::operator-(const VVector& v) const {
    return VPoint(_x - v.x(), _y - v.y());
}

VVector VPoint::operator-(const VPoint& p) const {
    return VVector(_x - p._x, _y - p._y);
}

VPoint VPoint::operator*(double factor) const {
    return VPoint(_x * factor, _y * factor);
}

VPoint VPoint::operator/(double factor) const {
    return VPoint(_x / factor, _y / factor);
}

bool VPoint::operator==(const VPoint& p) const
{
    return _x == p._x && _y == p._y;
}

bool VPoint::operator!=(const VPoint& p) const
{
    return _x != p._x || _y != p._y;
}

bool VPoint::operator<(const VPoint& p) const
{
    return _x < p._x || (_x == p._x && _y < p._y);
}

bool VPoint::operator<=(const VPoint& p) const
{
    return *this == p || *this < p;
}

bool VPoint::operator>(const VPoint& p) const
{
    return !(*this <= p);
}

bool VPoint::operator>=(const VPoint& p) const
{
    return *this == p || *this > p;
}

double VPoint::x() const
{
    return _x;
}

double VPoint::y() const
{
    return _y;
}

VVector VPoint::toVector() const
{
    return VVector(_x, _y);
}

VPoint VPoint::midPoint(const VPoint& p) const
{
    return VPoint((_x + p._x) / 2, (_y + p._y) / 2);
}

bool geometry::clockwise(const VPoint& a, const VPoint& b, const VPoint& c)
{
    VVector ab = b-a;
    VVector bc = c-b;
    
    return ab.x()*bc.y()-ab.y()*bc.x()<0;
}

bool geometry::pointsOnLine(const VPoint& a, const VPoint& b, const VPoint& c)
{
    VVector ab = b-a;
    VVector bc = c-b;
    
    return ab.x()*bc.y()-ab.y()*bc.x()==0;
}
