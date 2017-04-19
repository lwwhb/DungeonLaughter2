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
#include "Fortune.h"

using namespace voronoi;
using namespace voronoi::fortune;
using namespace geometry;

Fortune::Fortune()
: _diagram(0)
{
}

void Fortune::operator()(VoronoiDiagram& diagram)
{
    this->_diagram = &diagram;
    calculate();
    this->_diagram = 0;
}

void Fortune::calculate()
{
    addEventsFor(_diagram->sites());
    
    while (VEvent* event = nextEvent()) {
        processEvent(event);
        delete event;
    }
}

void Fortune::processEvent(VEvent* event)
{
    sweepLineY = event->position().y();
    
    if (event->isSiteEvent()) {
        handleSiteEvent(event->asSiteEvent());
    } else {
        handleCircleEvent(event->asCircleEvent());
    }
}

void Fortune::addEventsFor(const std::vector<VoronoiSite*>& sites)
{
    for (std::vector<VoronoiSite*>::const_iterator it = sites.begin(); it != sites.end(); ++it) {
        addEvent(new SiteEvent(*it));
    }
}

void Fortune::addEvent(VEvent* event)
{
    eventQueue.push(event);
}

VEvent* Fortune::nextEvent()
{
    if (eventQueue.empty()) {
        return 0;
    }
    
    VEvent* event = eventQueue.top();
    eventQueue.pop();
    
    return event;
}

void Fortune::handleSiteEvent(SiteEvent* event)
{
    VoronoiSite* site = event->site();
    
    Arc* newArc = beachLine.createArc(site);
    
    if (beachLine.isEmpty()) {
        beachLine.insert(newArc);
        return;
    }
    
    Arc* arc = beachLine.arcAbove(site->position());
    
    if (arc) {
        arc->invalidateCircleEvent();
        newArc->setLeftEdge(_diagram->createEdge(arc->site(), site));
        
        beachLine.splitArcWith(arc, newArc);
        //arc->splitWith(newArc);
    } else {
        Arc* last = beachLine.lastElement();
        newArc->setLeftEdge(_diagram->createEdge(last->site(), site));
        //last->insert(newArc);
        beachLine.insertAfter(newArc, last);
    }
    
    checkForCircleEvent(newArc->prev());
    checkForCircleEvent(newArc->next());
}

void Fortune::handleCircleEvent(CircleEvent* event)
{
    if (!event->isValid()) return;
    
    Arc* arc = event->arc();
    Arc* prev = arc->prev();
    Arc* next = arc->next();
    VoronoiEdge* leftEdge = arc->leftEdge();
    VoronoiEdge* rightEdge = arc->rightEdge();
    VPoint centerPoint = event->circle().center();
    
    VoronoiEdge* newEdge = _diagram->createEdge(prev->site(), next->site());
    
    newEdge->adjustOrientation(arc->site()->position());
    newEdge->addPoint(centerPoint);
    
    leftEdge->adjustOrientation(next->site()->position());
    leftEdge->addPoint(centerPoint);
    
    rightEdge->adjustOrientation(prev->site()->position());
    rightEdge->addPoint(centerPoint);
    
    // connect half edges
    VoronoiHalfEdge* he = leftEdge->halfEdgeFor(arc->site());
    he->setStartPoint(centerPoint);
    he->setPrev(rightEdge->halfEdgeFor(arc->site()));
    
    he = rightEdge->halfEdgeFor(next->site());
    he->setStartPoint(centerPoint);
    he->setPrev(newEdge->halfEdgeFor(next->site()));
    
    he = newEdge->halfEdgeFor(prev->site());
    he->setStartPoint(centerPoint);
    he->setPrev(leftEdge->halfEdgeFor(prev->site()));
    
    
    beachLine.replaceArc(arc, newEdge);
    
    checkForCircleEvent(prev);
    checkForCircleEvent(next);
}

void Fortune::checkForCircleEvent(Arc* arc)
{
    if (!arc) return;
    arc->invalidateCircleEvent();
    
    if (!arc->hasTwoDifferentNeighborSites()) return;
    
    VPoint a = arc->prev()->site()->position();
    VPoint b = arc->next()->site()->position();
    VPoint c = arc->site()->position();
    
    if (pointsOnLine(a, b, c)) {
        if (a.x() >= b.x() && b.x() >= c.x()) {
            
        } else {
            return;
        }
    } else {
        if (clockwise(a, b, c)) return;
    }
    
    Circle circle(a, b, c);
    
    if (circle.top().y() > sweepLineY) {
        return;
    }
    
    addEvent(new CircleEvent(arc, circle));
}
