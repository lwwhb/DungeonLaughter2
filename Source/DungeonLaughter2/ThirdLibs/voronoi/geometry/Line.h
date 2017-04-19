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

#pragma once

#include "VPoint.h"
#include "VVector.h"

namespace geometry {
    
    class LineIntersectionSolutionSet;
    
    class DUNGEONLAUGHTER2_API Line
    {
    public:
        enum Type {
            NULL_LINE,
            LINE,
            RAY,
            SEGMENT
        };
        
        Line();
        
        static Line forDirection(const VPoint& supportVector, const VVector& direction);
        static Line forNormal(const VPoint& supportVector, const VVector& normal);
        
        static Line ray(const VPoint& supportVector, const VVector& direction);
        static Line segment(const VPoint& point1, const VPoint& point2);
        
        const VPoint& supportVector() const;
        const VPoint& startPoint() const;
        const VVector& direction() const;
        const VPoint& endPoint() const;
        Type type() const;
        
        bool isNull() const;
        bool isLine() const;
        bool isRay() const;
        bool isSegment() const;
        
        Line asLine() const;
        
        void setStartPoint(const VPoint& point);
        void setEndPoint(const VPoint& point);
        void setDirection(const VVector& direction);
        
        void invertDirection();
        bool addPoint(const VPoint& point);
        
        LineIntersectionSolutionSet intersection(const Line& line) const;
        VVector normal() const;
        Line perpendicular(const VPoint& point) const;
        
        bool isParallelTo(const Line& line) const;
        bool contains(const VPoint& p) const;
        bool overlaps(const Line& line) const;
        bool sameSide(const VPoint& p1, const VPoint& p2) const;
    protected:
        VPoint _startPoint;
        VPoint _endPoint;
        VVector _direction;
        Type _type;
        
        Line(Type type, const VPoint& start, const VPoint& end, const VVector& direction);
        
        bool lineContains(const VPoint& p) const;
        bool intersectionCoefficient(const Line& line, double& coefficient) const;
        LineIntersectionSolutionSet lineIntersection(const Line& line) const;
        bool containsCoefficient(double coefficient) const;
        double coefficientForPointOnLine(const VPoint& p) const;
    };
    
    class DUNGEONLAUGHTER2_API LineIntersectionSolutionSet
    {
    public:
        enum Type {
            NO_SOLUTION,
            ONE_SOLUTION,
            INFINITE_SOLUTIONS
        };
        
        LineIntersectionSolutionSet();
        LineIntersectionSolutionSet(const VPoint& point);
        
        static LineIntersectionSolutionSet noSolution();
        static LineIntersectionSolutionSet infiniteSolutions();
        
        bool isEmpty() const;
        bool isOne() const;
        bool isInfinite() const;
        
        Type type() const;
        const VPoint& point() const;
    private:
        VPoint _point;
        Type _type;
    };
    
    
} //end namespace geometry
