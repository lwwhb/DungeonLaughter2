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

#include <vector>
#include <queue>

#include "../VoronoiDiagram.h"
#include "VEvent.h"
#include "BeachLine.h"

namespace voronoi {
    namespace fortune {
        
        class DUNGEONLAUGHTER2_API Fortune
        {
        public:
            Fortune();
            
            void operator()(VoronoiDiagram& diagram);
        private:
            VoronoiDiagram* _diagram;
            
            std::priority_queue<VEvent*, std::vector<VEvent*>, EventComparator> eventQueue;
            BeachLine beachLine;
            double sweepLineY;
            
            void calculate();
            void addEventsFor(const std::vector<VoronoiSite*>& sites);
            
            void addEvent(VEvent* event);
            VEvent* nextEvent();
            
            void processEvent(VEvent* event);
            
            void handleSiteEvent(SiteEvent* event);
            void handleCircleEvent(CircleEvent* event);
            
            void checkForCircleEvent(Arc* arc);
        };
        
    } //end namespace fortune
} //end namespace voronoi

