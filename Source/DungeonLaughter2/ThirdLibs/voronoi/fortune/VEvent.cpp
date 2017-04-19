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
#include "VEvent.h"

using namespace voronoi;
using namespace voronoi::fortune;
using namespace geometry;

VEvent::~VEvent()
{
}

bool VEvent::isSiteEvent() const
{
    return false;
}

bool VEvent::isCircleEvent() const
{
    return false;
}

bool VEvent::operator<(const VEvent& event) const {
    VPoint p1 = position();
    VPoint p2 = event.position();
    
    return p1.y()==p2.y() ? p1.x()>p2.x() : p1.y()<p2.y();
}

SiteEvent* VEvent::asSiteEvent() const
{
    return isSiteEvent() ? (SiteEvent*)this : 0;
}

CircleEvent* VEvent::asCircleEvent() const
{
    return isCircleEvent() ? (CircleEvent*)this : 0;
}

SiteEvent::SiteEvent(VoronoiSite* site) : _site(site)
{
}

bool SiteEvent::isSiteEvent() const
{
    return true;
}

VPoint SiteEvent::position() const
{
    return _site->position();
}

VoronoiSite* SiteEvent::site() const
{
    return _site;
}

CircleEvent::CircleEvent(Arc* arc, Circle circle) : _arc(arc), _circle(circle), valid(true)
{
    arc->resetCircleEvent(this);
}

bool CircleEvent::isCircleEvent() const
{
    return true;
}

VPoint CircleEvent::position() const
{
    return _circle.center()-VVector(0,_circle.radius());
}

bool CircleEvent::isValid() const
{
    return valid;
}

void CircleEvent::invalidate()
{
    valid = false;
}

Arc* CircleEvent::arc() const
{
    return _arc;
}

const Circle& CircleEvent::circle() const
{
    return _circle;
}

bool EventComparator::operator()(VEvent* event1, VEvent* event2) {
    return *event1 < *event2;
}
