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

#include "VVector.h"

namespace geometry {
    
    class DUNGEONLAUGHTER2_API VPoint
    {
    public:
        VPoint();
        VPoint(double x, double y);
        
        double x() const;
        double y() const;
        void setX(double x);
        void setY(double y);
        
        bool operator==(const VPoint& p) const;
        bool operator!=(const VPoint& p) const;
        bool operator<(const VPoint& p) const;
        bool operator<=(const VPoint& p) const;
        bool operator>(const VPoint& p) const;
        bool operator>=(const VPoint& p) const;
        
        VPoint operator-() const;
        VPoint& operator+=(const VVector& p);
        VPoint& operator-=(const VVector& v);
        VPoint& operator*=(double factor);
        VPoint& operator/=(double factor);
        VPoint operator+(const VVector& p) const;
        VVector operator-(const VPoint& p) const;
        VPoint operator-(const VVector& v) const;
        VPoint operator*(double factor) const;
        VPoint operator/(double factor) const;
        
        VVector toVector() const;
        
        VPoint midPoint(const VPoint& p) const;
    private:
        double _x;
        double _y;
    };
    
    DUNGEONLAUGHTER2_API VPoint operator*(double factor, const VPoint& p);
    
    DUNGEONLAUGHTER2_API bool clockwise(const VPoint& a, const VPoint& b, const VPoint& c);
    DUNGEONLAUGHTER2_API bool pointsOnLine(const VPoint& a, const VPoint& b, const VPoint& c);
    
} //end namespace geometry
