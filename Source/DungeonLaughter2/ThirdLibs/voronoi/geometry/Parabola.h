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

namespace geometry {
    
    class ParabolaIntersectionSolutionSet;
    
    class DUNGEONLAUGHTER2_API Parabola
    {
    public:
        Parabola();
        Parabola(const VPoint& focus, double baselineY);
        
        const VPoint& focus() const;
        double baselineY() const;
        
        double operator()(double x) const;
        VPoint at(double x) const;
        
        const VPoint& vertex() const;
        double stretchFactor() const;
        
        // parabola formula: (x-h)^2 = 4a(y-k) with vertex=(h,k) and a=dist(line, vertex)/2
        double a() const;
        double h() const;
        double k() const;
        
        ParabolaIntersectionSolutionSet intersection(const Parabola& parabola) const;
    protected:
        VPoint _focus;
        double _baselineY;
        
        double _stretchFactor;
        VPoint _vertex;
    };
    
    class DUNGEONLAUGHTER2_API ParabolaIntersectionSolutionSet
    {
    public:
        enum Type {
            NO_SOLUTION,
            ONE_SOLUTION,
            TWO_SOLUTIONS,
            INFINITE_SOLUTIONS
        };
        
        ParabolaIntersectionSolutionSet();
        ParabolaIntersectionSolutionSet(const VPoint& point);
        ParabolaIntersectionSolutionSet(const VPoint& point1, const VPoint& point2);
        
        static ParabolaIntersectionSolutionSet noSolution();
        static ParabolaIntersectionSolutionSet infiniteSolutions();
        
        bool isEmpty() const;
        bool isOne() const;
        bool isTwo() const;
        bool isInfinite() const;
        
        const Type& type() const;
        const VPoint& point() const;
        const VPoint& leftPoint() const;
        const VPoint& rightPoint() const;
    private:
        Type _type;
        VPoint _leftPoint;
        VPoint _rightPoint;
    };
    
    
} //end namespace geometry
