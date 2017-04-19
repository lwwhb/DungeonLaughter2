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
    
    class DUNGEONLAUGHTER2_API Rectangle
    {
    public:
        Rectangle();
        Rectangle(const VPoint& a, const VPoint& b);
        Rectangle(double left, double top, double width, double height);
        
        double x() const;
        double y() const;
        double width() const;
        double height() const;
        
        double left() const;
        double right() const;
        double top() const;
        double bottom() const;
        
        VPoint topLeft() const;
        VPoint topRight() const;
        VPoint bottomLeft() const;
        VPoint bottomRight() const;
        
        bool contains(const VPoint& p) const;
        void resizeToInclude(const VPoint& p);
    protected:
        double _top;
        double _left;
        double _width;
        double _height;
    };
    
} //end namespace geometry
