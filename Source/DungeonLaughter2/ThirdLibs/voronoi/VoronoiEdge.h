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

#include "geometry/VPoint.h"
#include "geometry/Line.h"
#include "geometry/Rectangle.h"
#include "geometry/ConvexPolygon.h"

#include "VoronoiSite.h"

namespace voronoi {
    
    class VoronoiHalfEdge;
    
    class DUNGEONLAUGHTER2_API VoronoiEdge
    {
    public:
        VoronoiEdge(VoronoiSite* left, VoronoiSite* right);
        ~VoronoiEdge();
        
        void addPoint(const geometry::VPoint& point);
        void adjustOrientation(const geometry::VPoint& awayPoint);
        
        VoronoiHalfEdge* halfEdgeFor(VoronoiSite* site);
        
        geometry::Line getRenderLine(const geometry::Rectangle& boundingBox) const;
        geometry::Line getRenderLine(const geometry::ConvexPolygon& boundingPolygon) const;
    protected:
        VoronoiSite* _left;
        VoronoiSite* _right;
        
        geometry::Line _line;
        
        VoronoiHalfEdge* _halfEdge1;
        VoronoiHalfEdge* _halfEdge2;
    };
    
    class DUNGEONLAUGHTER2_API VoronoiHalfEdge
    {
    public:
        VoronoiHalfEdge(VoronoiSite* site);
        
        VoronoiSite* site() const;
        VoronoiHalfEdge* opposite() const;
        VoronoiHalfEdge* next() const;
        VoronoiHalfEdge* prev() const;
        const geometry::VPoint& startPoint() const;
        bool hasStartPoint() const;
        
        void setStartPoint(const geometry::VPoint& point);
        void setOpposite(VoronoiHalfEdge* opposite);
        void setNext(VoronoiHalfEdge* next);
        void setPrev(VoronoiHalfEdge* prev);
        
        VoronoiHalfEdge* begin();
    protected:
        VoronoiSite* _site;
        VoronoiHalfEdge* _prev;
        VoronoiHalfEdge* _next;
        VoronoiHalfEdge* _opposite;
        geometry::VPoint _startPoint;
        bool _hasStart;
    };
    
} //end namespace voronoi

