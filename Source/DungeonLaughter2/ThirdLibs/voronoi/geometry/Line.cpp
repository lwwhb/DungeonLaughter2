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

#include "Line.h"

using namespace geometry;

Line::Line() : _type(NULL_LINE)
{
}

Line::Line(Type type, const VPoint& start, const VPoint& end, const VVector& direction) : _startPoint(start), _endPoint(end), _direction(direction), _type(type)
{
}

Line Line::segment(const VPoint& point1, const VPoint& point2) {
    return Line(SEGMENT, point1, point2, point2-point1);
}

Line Line::ray(const VPoint& supportVector, const VVector& direction) {
    return Line(RAY, supportVector, supportVector+direction, direction);
}

Line Line::forDirection(const VPoint& supportVector, const VVector& direction)
{
    return Line(LINE, supportVector, supportVector+direction, direction);
}

Line Line::forNormal(const VPoint& supportVector, const VVector& normal)
{
    return Line::forDirection(supportVector, normal.perpendicular());
}

const VPoint& Line::supportVector() const
{
    return _startPoint;
}

const VPoint& Line::startPoint() const
{
    return _startPoint;
}

const VVector& Line::direction() const
{
    return _direction;
}

const VPoint& Line::endPoint() const
{
    return _endPoint;
}

Line::Type Line::type() const
{
    return _type;
}

void Line::setStartPoint(const VPoint& point)
{
    _startPoint = point;
    _direction = _endPoint - _startPoint;
}

void Line::setEndPoint(const VPoint& point)
{
    _endPoint = point;
    _direction = _endPoint - _startPoint;
}

void Line::setDirection(const VVector& direction)
{
    _direction = direction;
    _endPoint = _startPoint + _direction;
}

bool Line::isNull() const
{
    return _type==NULL_LINE;
}

bool Line::isLine() const
{
    return _type==LINE;
}

bool Line::isRay() const
{
    return _type==RAY;
}

bool Line::isSegment() const
{
    return _type==SEGMENT;
}

Line Line::asLine() const
{
    return Line::forDirection(_startPoint, _direction);
}

void Line::invertDirection()
{
    _direction *= -1;
    if (isRay()) {
        _endPoint = _startPoint+_direction;
    } else {
        std::swap(_startPoint, _endPoint);
    }
}

bool Line::addPoint(const VPoint& point)
{
    switch (_type) {
        case LINE:
            _startPoint = point;
            _type = RAY;
            return true;
        case RAY:
            _endPoint = point;
            _direction = _endPoint-_startPoint;
            _type = SEGMENT;
            return true;
        default:
            return false;
    }
}

LineIntersectionSolutionSet Line::intersection(const Line& line) const
{
    double s;
    double t;
    if (!intersectionCoefficient(line, s) || !line.intersectionCoefficient(*this, t)) {
        return overlaps(line) ? LineIntersectionSolutionSet::infiniteSolutions() : LineIntersectionSolutionSet::noSolution();
    }
    
    if (!containsCoefficient(s) || !line.containsCoefficient(t)) {
        return LineIntersectionSolutionSet::noSolution();
    }
    
    return LineIntersectionSolutionSet(_startPoint+s*_direction);
}

LineIntersectionSolutionSet Line::lineIntersection(const Line& line) const
{
    double s;
    if (!intersectionCoefficient(line, s)) {
        return lineContains(line.supportVector()) ? LineIntersectionSolutionSet::infiniteSolutions() : LineIntersectionSolutionSet::noSolution();
    }
    
    return LineIntersectionSolutionSet(_startPoint+s*_direction);
}

bool Line::intersectionCoefficient(const Line& line, double& coefficient) const
{
    VVector u = _direction;
    VVector v = line._direction;
    VVector w = _startPoint-line._startPoint;
    double denominator = v.x()*u.y()-v.y()*u.x();
    if (denominator==0) {
        return false;
    }
    coefficient= (v.y()*w.x()-v.x()*w.y())/denominator;
    return true;
}

bool Line::containsCoefficient(double coefficient) const
{
    switch (_type) {
        case LINE:
            return true;
        case RAY:
            return coefficient>=0;
        case SEGMENT:
            return coefficient>=0 && coefficient<=1;
        default:
            return false;
    }
}

VVector Line::normal() const
{
    return _direction.perpendicular();
}

Line Line::perpendicular(const VPoint& point) const
{
    return Line::forDirection(point, normal());
}

bool Line::lineContains(const VPoint& p) const
{
    return _direction.isParallelTo(p - _startPoint);
}

bool Line::isParallelTo(const Line& line) const
{
    return _direction.isParallelTo(line.direction());
}

bool Line::contains(const VPoint& p) const
{
    if (!lineContains(p)) {
        return false;
    }
    
    return containsCoefficient(coefficientForPointOnLine(p));
}

bool Line::overlaps(const Line& line) const
{
    if (!isParallelTo(line) || !lineContains(line.startPoint())) {
        return false;
    }
    
    return
    containsCoefficient(coefficientForPointOnLine(line.startPoint())) ||
    containsCoefficient(coefficientForPointOnLine(line.endPoint())) ||
    line.containsCoefficient(line.coefficientForPointOnLine(_startPoint)) ||
    line.containsCoefficient(line.coefficientForPointOnLine(_endPoint));
}

bool Line::sameSide(const VPoint& p1, const VPoint& p2) const
{
    VVector v1 = p1-lineIntersection(perpendicular(p1)).point();
    VVector v2 = p2-lineIntersection(perpendicular(p2)).point();
    
    return v1.dotProduct(v2)>0;
}

double Line::coefficientForPointOnLine(const VPoint& p) const
{
    double directionLength = _direction.length();
    
    if (directionLength <= 0) {
        return 0;
    }
    
    return (p - _startPoint).length() / directionLength;
}


LineIntersectionSolutionSet::LineIntersectionSolutionSet() : _type(NO_SOLUTION)
{
}

LineIntersectionSolutionSet::LineIntersectionSolutionSet(const VPoint& point) : _point(point), _type(ONE_SOLUTION)
{
}

LineIntersectionSolutionSet LineIntersectionSolutionSet::noSolution()
{
    return LineIntersectionSolutionSet();
}

LineIntersectionSolutionSet LineIntersectionSolutionSet::infiniteSolutions()
{
    LineIntersectionSolutionSet solutionSet;
    solutionSet._type = INFINITE_SOLUTIONS;
    return solutionSet;
}

bool LineIntersectionSolutionSet::isEmpty() const
{
    return _type==NO_SOLUTION;
}

bool LineIntersectionSolutionSet::isOne() const
{
    return _type==ONE_SOLUTION;
}

bool LineIntersectionSolutionSet::isInfinite() const
{
    return _type==INFINITE_SOLUTIONS;
}

LineIntersectionSolutionSet::Type LineIntersectionSolutionSet::type() const
{
    return _type;
}

const VPoint& LineIntersectionSolutionSet::point() const
{
    return _point;
}
