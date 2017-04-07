// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "IslandMap.h"

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
}
void IslandMap::generateSquarePoints(int seed)
{
}
void IslandMap::generateHexagonPoints(int seed)
{
}
