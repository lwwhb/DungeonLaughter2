// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "IslandMap.h"
#include "../ThirdLibs/voronoi/VoronoiDiagram.h"
#include "../ThirdLibs/voronoi/VoronoiSite.h"
#include "../ThirdLibs/voronoi/VoronoiEdge.h"
#include "../ThirdLibs/voronoi/VoronoiCell.h"

IslandMap::IslandMap(int size, EIslandShapeType islandShapeType, EPointType pointType, int numPoints)
{
    m_nSize = size;
    m_IslandShapeType = islandShapeType;
    m_PointType = pointType;
    m_nNumPoints = numPoints;
}

IslandMap::~IslandMap()
{
}

void IslandMap::reset()
{
    // Break cycles so the garbage collector will release data.
    m_Points.clear();

    for(size_t i = 0; i<m_Edges.size(); ++i)
    {
        if(m_Edges[i] != nullptr)
        {
            if(m_Edges[i]->m_pD0 != nullptr)
            {
                delete m_Edges[i]->m_pD0;
                m_Edges[i]->m_pD0 = nullptr;
            }
            if(m_Edges[i]->m_pD1 != nullptr)
            {
                delete m_Edges[i]->m_pD1;
                m_Edges[i]->m_pD1 = nullptr;
            }
            if(m_Edges[i]->m_pV0 != nullptr)
            {
                delete m_Edges[i]->m_pV0;
                m_Edges[i]->m_pV0 = nullptr;
            }
            if(m_Edges[i]->m_pV1 != nullptr)
            {
                delete m_Edges[i]->m_pV1;
                m_Edges[i]->m_pV1 = nullptr;
            }
            delete m_Edges[i];
            m_Edges[i] = nullptr;
        }
    }
    m_Edges.clear();
    
    for(size_t i = 0; i<m_Centers.size(); ++i)
    {
        if(m_Centers[i] != nullptr)
        {
            m_Centers[i]->m_Neighbors.clear();
            m_Centers[i]->m_Corners.clear();
            m_Centers[i]->m_Borders.clear();
            
            delete m_Centers[i];
            m_Centers[i] = nullptr;
        }
    }
    m_Centers.clear();
    
    for(size_t i = 0; i<m_Corners.size(); ++i)
    {
        if(m_Corners[i] != nullptr)
        {
            if(m_Corners[i]->m_pDownslope != nullptr)
            {
                delete m_Corners[i]->m_pDownslope;
                m_Corners[i]->m_pDownslope = nullptr;
            }
            if(m_Corners[i]->m_pWatershed != nullptr)
            {
                delete m_Corners[i]->m_pWatershed;
                m_Corners[i]->m_pWatershed = nullptr;
            }
            m_Corners[i]->m_Touches.clear();
            m_Corners[i]->m_Protrudes.clear();
            m_Corners[i]->m_Adjacent.clear();
            
            delete m_Corners[i];
            m_Corners[i] = nullptr;
        }
    }
    m_Corners.clear();
}
bool IslandMap::placePoints(int seed)
{
    //Generate the initial random set of points
    switch (m_PointType) {
        case EPointType::PTE_Random:
            generateRandomPoints(seed);
            break;
        case EPointType::PTE_Relaxed:
            generateRelaxedPoints(seed);
            break;
        case EPointType::PTE_Square:
            generateSquarePoints(seed);
            break;
        case EPointType::PTE_Hexagon:
            generateHexagonPoints(seed);
            break;
        default:
            break;
    }
    return true;
}
void IslandMap::generateRandomPoints(int seed)
{
    PM_PRNG mapRandom(seed);
    for (int i = 0; i < m_nNumPoints; ++i) {
        FVector2D point(mapRandom.nextDoubleRange(10, m_nSize-10),
                      mapRandom.nextDoubleRange(10, m_nSize-10));
        m_Points.push_back(point);
    }
}
void IslandMap::generateRelaxedPoints(int seed)
{
    // We'd really like to generate "blue noise". Algorithms:
    // 1. Poisson dart throwing: check each new point against all
    //     existing points, and reject it if it's too close.
    // 2. Start with a hexagonal grid and randomly perturb points.
    // 3. Lloyd Relaxation: move each point to the centroid of the
    //     generated Voronoi polygon, then generate Voronoi again.
    // 4. Use force-based layout algorithms to push points away.
    // 5. More at http://www.cs.virginia.edu/~gfx/pubs/antimony/
    // Option 3 is implemented here. If it's run for too many iterations,
    // it will turn into a grid, but convergence is very slow, and we only
    // run it a few times.
    
    std::vector<FVector2D> randomPoints;
    PM_PRNG mapRandom(seed);
    for (int i = 0; i < m_nNumPoints; ++i) {
        FVector2D point(mapRandom.nextDoubleRange(10, m_nSize-10),
                        mapRandom.nextDoubleRange(10, m_nSize-10));
        randomPoints.push_back(point);
    }
    voronoi::VoronoiDiagram     diagram;
    int num_lloyd_relaxations = 2;
    for (int i = 0; i<num_lloyd_relaxations; ++i) {
        diagram.reset();
        
        std::vector<voronoi::VoronoiSite*>   sites;
        
        for(int j = 0; j<randomPoints.size(); ++j )
            sites.push_back(new voronoi::VoronoiSite(randomPoints[j].X, randomPoints[j].Y));
        
        voronoi::VoronoiDiagram::removeDuplicates(sites);
        
        diagram.initialize(sites);
        
        diagram.calculate();
        
        for (std::vector<voronoi::VoronoiSite*>::iterator it = sites.begin(); it != sites.end(); ++it) {
            delete *it;
        }
        sites.clear();
    }
    
    for (std::vector<voronoi::VoronoiSite*>::iterator it = diagram.sites().begin(); it !=diagram.sites().end(); ++it)
    {
        FVector2D point((*it)->position().x(), (*it)->position().y());
        m_Points.push_back(point);
    }
}
void IslandMap::generateSquarePoints(int seed)
{
    int n = sqrt(m_nNumPoints);
    for (int x = 0; x < n; ++x)
    {
        for (int y = 0; y < n; ++y)
        {
            FVector2D point((0.5f + x)/n * m_nSize, (0.5f + y)/n * m_nSize);
            m_Points.push_back(point);
        }
    }
}
void IslandMap::generateHexagonPoints(int seed)
{
    int n = sqrt(m_nNumPoints);
    for (int x = 0; x < n; ++x)
    {
        for (int y = 0; y < n; ++y)
        {
            FVector2D point((0.5f + x)/n * m_nSize, (0.25f + 0.5f * (x % 2) + y)/n * m_nSize);
            m_Points.push_back(point);
        }
    }
}
