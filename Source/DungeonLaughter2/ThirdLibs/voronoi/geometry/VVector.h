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

namespace geometry {
    
    class VPoint;
    
    class DUNGEONLAUGHTER2_API VVector
    {
    public:
        VVector();
        VVector(double x, double y);
        
        double x() const;
        double y() const;
        void setX(double x);
        void setY(double y);
        
        bool operator==(const VVector& v) const;
        bool operator!=(const VVector& v) const;
        
        VVector operator-() const;
        VVector& operator+=(const VVector& v);
        VVector& operator-=(const VVector& v);
        VVector& operator*=(double factor);
        VVector& operator/=(double factor);
        VVector operator+(const VVector& v) const;
        VVector operator-(const VVector& v) const;
        VVector operator*(double factor) const;
        VVector operator/(double factor) const;
        
        VPoint toPoint() const;
        
        double dotProduct(const VVector& v) const;
        double length() const;
        double squaredLength() const;
        
        bool isParallelTo(const VVector& v) const;
        
        VVector perpendicular() const;
    private:
        double _x;
        double _y;
    };
    
    DUNGEONLAUGHTER2_API VVector operator*(double factor, const VVector& v);
    
} //end namespace geometry
