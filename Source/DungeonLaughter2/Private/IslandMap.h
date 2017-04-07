// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "IslandGenerator.h"

class VEdge;
class VCenter;

class VCorner
{
public:
    int         m_nIndex;
    FVector2D   m_Point;        // location
    bool        m_bWater;       // lake or ocean
    bool        m_bOcean;       // ocean
    bool        m_bCoast;       // land polygon touching an ocean
    bool        m_bBorder;      // at the edge of the map
    FString     m_bBiome;       // biome type (see article)
    float       m_bElevation;   // 0.0-1.0
    float       m_bMoisture;    // 0.0-1.0
    
    std::vector<VCenter*>    m_Touches;
    std::vector<VEdge*>      m_Protrudes;
    std::vector<VCorner*>    m_Adjacent;
    
    int         m_nRiver;       // 0 if no river, or volume of water in river
    VCorner*    m_pDownslope;    // pointer to adjacent corner most downhill
    VCorner*    m_pWatershed;    // pointer to coastal corner, or null
    int         m_nWatershed_size;
};

class VCenter
{
public:
    int         m_nIndex;
    FVector2D   m_Point;        // location
    bool        m_bWater;       // lake or ocean
    bool        m_bOcean;       // ocean
    bool        m_bCoast;       // land polygon touching an ocean
    bool        m_bBorder;      // at the edge of the map
    FString     m_bBiome;       // biome type (see article)
    float       m_bElevation;   // 0.0-1.0
    float       m_bMoisture;    // 0.0-1.0
    
    std::vector<VCenter*>       m_Neighbors;
    std::vector<VEdge*>         m_Borders;
    std::vector<VCorner*>       m_Corners;
};

class VEdge
{
public:
    int         m_nIndex;
    VCenter*    m_pD0;          // Delaunay edge
    VCenter*    m_pD1;          // Delaunay edge
    VCorner*    m_pV0;          // Voronoi edge
    VCorner*    m_pV1;          // Voronoi edge
    FVector2D   m_Midpoint;     // halfway between v0,v1
    int         m_nRiver;       // volume of water, or 0
};

/**
 * 岛屿地图
 */
class IslandMap
{
public:
	IslandMap(int size, EIslandShapeType islandShapeType, EPointType pointType, int numPoints);
	~IslandMap();

    void reset();
    
    bool placePoints(int seed);
private:
    void generateRandomPoints(int seed);
    void generateRelaxedPoints(int seed);
    void generateSquarePoints(int seed);
    void generateHexagonPoints(int seed);
private:
    int                 m_nSize;
    EIslandShapeType    m_IslandShapeType;
    EPointType          m_PointType;
    int                 m_nNumPoints;
    
    std::vector<FVector2D>      m_Points;
    std::vector<VCenter*>       m_Centers;
    std::vector<VCorner*>       m_Corners;
    std::vector<VEdge*>         m_Edges;
};
