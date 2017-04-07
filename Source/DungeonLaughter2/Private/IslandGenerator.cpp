// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "IslandGenerator.h"
#include <math.h>
#include <algorithm>
#include "IslandMap.h"

IslandGenerator* g_pIslandGeneratorInstance = nullptr;
IslandGenerator* IslandGenerator::getInstance()
{
    if (g_pIslandGeneratorInstance == nullptr)
        g_pIslandGeneratorInstance = new IslandGenerator();
    
    return g_pIslandGeneratorInstance;
}
void IslandGenerator::releaseInstance()
{
    if (g_pIslandGeneratorInstance)
    {
        delete g_pIslandGeneratorInstance;
        g_pIslandGeneratorInstance = nullptr;
    }
}
IslandGenerator::IslandGenerator()
{
    m_fIslandFactor = 1.07f; // 1.0 means no small islands; 2.0 leads to a lot
    
    m_pIslandMap = nullptr;
}

IslandGenerator::~IslandGenerator()
{
}
bool IslandGenerator::generate(int size, EIslandShapeType islandShapeType, EPointType pointType, int numPoints)
{
    if(m_pIslandMap != nullptr)
    {
        m_pIslandMap->reset();
        delete m_pIslandMap;
        m_pIslandMap = nullptr;
    }
    
    if(m_pIslandMap == nullptr)
    {
        m_pIslandMap = new IslandMap(size, islandShapeType, pointType, numPoints);
        if(!m_pIslandMap)
        {
            UE_LOG(LogTemp, Fatal, TEXT("Shaping map failed!"));
            return false;
        }
        if(!m_pIslandMap->placePoints(795802))
        {
            UE_LOG(LogTemp, Fatal, TEXT("Placing points failed!"));
            return false;
        }
    }
    return true;
}
