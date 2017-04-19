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
    
    class DUNGEONLAUGHTER2_API Circle
    {
    public:
        Circle();
        Circle(const VPoint& center, double radius);
        Circle(const VPoint& a, const VPoint& b, const VPoint& c);
        
        const VPoint& center() const;
        double radius() const;
        
        bool contains(const VPoint& p) const;
        
        VPoint top() const;
        VPoint bottom() const;
        VPoint left() const;
        VPoint right() const;
    protected:
        VPoint _center;
        double _radius;
    };
    
} //end namespace geometry
