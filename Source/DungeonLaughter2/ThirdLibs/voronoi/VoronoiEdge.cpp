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
#include "VoronoiEdge.h"

using namespace voronoi;
using namespace geometry;

VoronoiEdge::VoronoiEdge(VoronoiSite* left, VoronoiSite* right) : _left(left), _right(right)
{
    _line = Line::forNormal(_left->position().midPoint(_right->position()),_left->position()-_right->position());
    _halfEdge1 = new VoronoiHalfEdge(_left);
    _halfEdge2 = new VoronoiHalfEdge(_right);
    _halfEdge1->setOpposite(_halfEdge2);
}

VoronoiEdge::~VoronoiEdge()
{
    delete _halfEdge1;
    delete _halfEdge2;
}

VoronoiHalfEdge* VoronoiEdge::halfEdgeFor(VoronoiSite* site)
{
    if (site == _halfEdge1->site()) return _halfEdge1;
    if (site == _halfEdge2->site()) return _halfEdge2;
    return 0;
}

void VoronoiEdge::addPoint(const VPoint& point)
{
    _line.addPoint(point);
}

void VoronoiEdge::adjustOrientation(const VPoint& awayPoint)
{
    if (Line::segment(_left->position(), _right->position()).sameSide(_line.startPoint()+_line.direction(), awayPoint)) {
        _line.invertDirection();
    }
}

Line VoronoiEdge::getRenderLine(const Rectangle& boundingBox) const
{
    ConvexPolygon boundingPolygon;
    
    boundingPolygon
    << boundingBox.topLeft()
    << boundingBox.topRight()
    << boundingBox.bottomRight()
    << boundingBox.bottomLeft();
    
    return getRenderLine(boundingPolygon);
}

Line VoronoiEdge::getRenderLine(const ConvexPolygon& boundingPolygon) const
{
    if (_line.isSegment() && boundingPolygon.contains(_line.startPoint()) && boundingPolygon.contains(_line.endPoint())) {
        return _line;
    }
    
    const PolygonIntersectionSolutionSet solutionSet = boundingPolygon.intersection(_line);
    
    switch (solutionSet.size()) {
        case 0:
            return Line();
        case 1:
            if (boundingPolygon.contains(_line.startPoint())) {
                return Line::segment(_line.startPoint(), solutionSet.points().front());
            } else {
                return Line::segment(solutionSet.points().front(), _line.endPoint());
            }
        default:
            const std::vector<VPoint> points = solutionSet.points();
            
            return Line::segment(points.front(), points.back());
    }
}


VoronoiHalfEdge::VoronoiHalfEdge(VoronoiSite* site) : _site(site), _prev(0), _next(0), _opposite(0), _hasStart(false)
{
}

VoronoiSite* VoronoiHalfEdge::site() const
{
    return _site;
}

VoronoiHalfEdge* VoronoiHalfEdge::opposite() const
{
    return _opposite;
}

VoronoiHalfEdge* VoronoiHalfEdge::next() const
{
    return _next;
}

VoronoiHalfEdge* VoronoiHalfEdge::prev() const
{
    return _prev;
}

const geometry::VPoint& VoronoiHalfEdge::startPoint() const
{
    return _startPoint;
}

bool VoronoiHalfEdge::hasStartPoint() const
{
    return _hasStart;
}

void VoronoiHalfEdge::setOpposite(VoronoiHalfEdge* opposite)
{
    _opposite = opposite;
    _opposite->_opposite = this;
}

void VoronoiHalfEdge::setNext(VoronoiHalfEdge* next)
{
    _next = next;
    _next->_prev = this;
}

void VoronoiHalfEdge::setPrev(VoronoiHalfEdge* prev)
{
    _prev = prev;
    _prev->_next = this;
}

void VoronoiHalfEdge::setStartPoint(const geometry::VPoint& point)
{
    _startPoint = point;
    if (!_hasStart) _hasStart = true;
}

VoronoiHalfEdge* VoronoiHalfEdge::begin()
{
    VoronoiHalfEdge* start = this;
    VoronoiHalfEdge* he = start;
    while (he->_prev) {
        he = he->_prev;
        if (he==start) break;
    }
    if (!he->_hasStart) he = he->next();
    return he;
}
