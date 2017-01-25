// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "DungeonGenerator.h"
#include "Area.h"
#include <algorithm>

DungeonGenerator* g_pDungeonGeneratorInstance = nullptr;
DungeonGenerator* DungeonGenerator::getInstance()
{
	if (g_pDungeonGeneratorInstance == nullptr)
		g_pDungeonGeneratorInstance = new DungeonGenerator();

	return g_pDungeonGeneratorInstance;
}
DungeonGenerator::DungeonGenerator()
	:m_pAreaEntrance(nullptr), m_pAreaExit(nullptr)
{
	m_nSpecialAreaCount = 0;
	m_nMainPathAreasCount = 0;
	m_nSidePathAreasCount = 0;
	m_nSecondaryAreasCount = 0;
}

DungeonGenerator::~DungeonGenerator()
{
}
bool DungeonGenerator::setGeneratorSetting(int minSplitAreaSize, int maxSplitAreaSize, int minAreaSize, int minSpecialAreaSize, 
	bool doublePath, float secondaryAreaRatio)
{
	if (minSplitAreaSize > maxSplitAreaSize)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("MinSplitAreaSize must be less than MaxSplitAreaSize or Equal.")));
		return false;
	}
	if (minAreaSize * 2 + 1 > minSplitAreaSize)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("MinAreaSize must be less than half of MinSplitAreaSize, MinAreaSize < MinSplitAreaSize/2.")));
		return false;
	}
	if (minSpecialAreaSize <= minAreaSize)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("MinSpecialAreaSize value must be more than MinAreaSize value.")));
		return false;
	}
	if (secondaryAreaRatio < 0.0f || secondaryAreaRatio > 0.5f)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("SecondaryAreaRatio value must between 0.0f~0.5f")));
		return false;
	}
	m_nMinSplitAreaSize = minSplitAreaSize;
	m_nMaxSplitAreaSize = maxSplitAreaSize;
	m_nMinAreaSize = minAreaSize; ///最小区域大小
	m_nMinSpecialAreaSize = minSpecialAreaSize; ///最小特殊区域大小
	m_bDoublePath = doublePath;
	m_fSecondaryAreaRatio = secondaryAreaRatio;
	return true;
}
bool DungeonGenerator::generateDungeon(int Width, int Height)
{
	m_nWidth = Width;
	m_nHeight = Height;
	if (!initAreas(FBox2D(FVector2D(0, 0), FVector2D(Width, Height))))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Init areas failed!"));
		return false;
	}
	if (!connectArea())
	{
		UE_LOG(LogTemp, Fatal, TEXT("Connect areas failed!"));
		return false;
	}
	return true;
}

std::vector<PathGraphNode*>& DungeonGenerator::getAreas()
{
	return m_Areas;
}
std::vector<Area*>& DungeonGenerator::getConnectedAreas()
{
	return m_ConnectedAreas;
}
bool DungeonGenerator::initAreas(const FBox2D& rect)
{
	if (!m_ConnectedAreas.empty())
		m_ConnectedAreas.clear();
	if (!m_Areas.empty())
	{
		std::vector<PathGraphNode*>::iterator iter;
		for (iter = m_Areas.begin(); iter != m_Areas.end(); iter++)
		{
			if (*iter)
			{
				delete *iter;
				*iter = nullptr;
			}
		}
		m_Areas.clear();
	}
	
	splitArea(rect);

	if (m_Areas.size() < 8) {
		return false;
	}
	for (int i = 0; i<m_Areas.size(); i++) {
		for (int j = i + 1; j<m_Areas.size(); j++) {
			Area* area1 = static_cast<Area*>(m_Areas[i]);
			Area* area2 = static_cast<Area*>(m_Areas[j]);
			if (area1 && area2)
				area1->addNeigbourArea(area2);
		}
	}
	return true;
}
void DungeonGenerator::splitArea(const FBox2D& rect)
{
	int w = rect.GetSize().X;
	int h = rect.GetSize().Y;

	if (w > m_nMaxSplitAreaSize && h < m_nMinSplitAreaSize) {
		int vw = FMath::RandRange((int)(rect.Min.X) + m_nMinAreaSize, (int)(rect.Max.X) - m_nMinAreaSize);
		splitArea(FBox2D(rect.Min, FVector2D(vw, rect.Max.Y)));
		splitArea(FBox2D(FVector2D(vw, rect.Min.Y), rect.Max));
	}
	else if (h > m_nMaxSplitAreaSize && w < m_nMinSplitAreaSize)
	{
		int vh = FMath::RandRange((int)(rect.Min.Y) + m_nMinAreaSize, (int)(rect.Max.Y) - m_nMinAreaSize);
		splitArea(FBox2D(rect.Min, FVector2D(rect.Max.X, vh)));
		splitArea(FBox2D(FVector2D(rect.Min.X, vh), rect.Max));
	}
	else if ((FMath::FRandRange(0.0f,1.0f) <= ((float)(m_nMinSplitAreaSize * m_nMinSplitAreaSize) / (w*h)) && w <= m_nMaxSplitAreaSize && h <= m_nMaxSplitAreaSize) || w < m_nMinSplitAreaSize || h < m_nMinSplitAreaSize)
	{
		Area* area = new(std::nothrow) Area();
		if (area)
		{
			area->setRect(rect);
			m_Areas.push_back(area);
		}
	}
	else
	{
		if (FMath::FRandRange(0.0f, 1.0f) < (float)(w - 2) / (w + h - 4))
		{
			int vw = FMath::RandRange((int)(rect.Min.X) + m_nMinAreaSize, (int)(rect.Max.X) - m_nMinAreaSize);
			splitArea(FBox2D(rect.Min, FVector2D(vw, rect.Max.Y)));
			splitArea(FBox2D(FVector2D(vw, rect.Min.Y), rect.Max));
		}
		else
		{
			int vh = FMath::RandRange((int)(rect.Min.Y) + m_nMinAreaSize, (int)(rect.Max.Y) - m_nMinAreaSize);
			splitArea(FBox2D(rect.Min, FVector2D(rect.Max.X, vh)));
			splitArea(FBox2D(FVector2D(rect.Min.X, vh), rect.Max));
		}
	}
}
bool DungeonGenerator::connectArea()
{
	int distance = 0;
	int retry = 0;
	int minDistance = (int)sqrt(m_Areas.size());
	do
	{
		do
		{
			int rand = FMath::RandRange(0, (int)(m_Areas.size()) - 1);
			m_pAreaEntrance = static_cast<Area*>(m_Areas[rand]);
		} while (m_pAreaEntrance->getRect().GetSize().X < 4 || m_pAreaEntrance->getRect().GetSize().Y < 4);
		do
		{
			int rand = FMath::RandRange(0, (int)(m_Areas.size()) - 1);
			m_pAreaExit = static_cast<Area*>(m_Areas[rand]);
		} while (m_pAreaExit == m_pAreaEntrance || m_pAreaExit->getRect().GetSize().X < 4 || m_pAreaExit->getRect().GetSize().Y < 4);

		PathGraph::buildDistanceMap(m_Areas, m_pAreaExit);

		distance = m_pAreaEntrance->getDistance();
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("retry time = %d"), retry++);
#endif // WITH_EDITOR
	} while (distance < minDistance);

	m_pAreaEntrance->setAreaType(EAreaTypeEnum::ATE_Entrance);
	m_pAreaEntrance->setAreaTypeMask(EAreaTypeMaskEnum::ATME_Entrance);

	m_pAreaExit->setAreaType(EAreaTypeEnum::ATE_Exit);
	m_pAreaExit->setAreaTypeMask(EAreaTypeMaskEnum::ATME_Exit);

	///生成第一条路径
	m_ConnectedAreas.push_back(m_pAreaEntrance);
	PathGraph::buildDistanceMap(m_Areas, m_pAreaExit);

	std::vector<PathGraphNode*> path = PathGraph::buildPath(m_Areas, m_pAreaEntrance, m_pAreaExit);
	Area* startArea = m_pAreaEntrance;
	for (PathGraphNode* node : path) {
		Area* next = static_cast<Area*>(node);
		if (next)
		{
			startArea->connectArea(next);
			startArea = next;
			if (startArea->getAreaType() == EAreaTypeEnum::ATE_Unknown && startArea->getAreaTypeMask() == EAreaTypeMaskEnum::ATME_Unknown)
			{
				startArea->setAreaTypeMask(EAreaTypeMaskEnum::ATME_MainPath);
				m_nMainPathAreasCount++;
			}
			m_ConnectedAreas.push_back(startArea);
		}
	}
	PathGraph::setWeight(path, m_pAreaEntrance->getDistance());
	
	///生成第二条路径
	if (m_bDoublePath)
	{
		PathGraph::buildDistanceMap(m_Areas, m_pAreaExit);
		path = PathGraph::buildPath(m_Areas, m_pAreaEntrance, m_pAreaExit);

		startArea = m_pAreaEntrance;
		for (PathGraphNode* node : path) {
			Area* next = static_cast<Area*>(node);
			if (next)
			{
				startArea->connectArea(next);
				startArea = next;
				if (startArea->getAreaType() == EAreaTypeEnum::ATE_Unknown && startArea->getAreaTypeMask() == EAreaTypeMaskEnum::ATME_Unknown)
				{
					startArea->setAreaTypeMask(EAreaTypeMaskEnum::ATME_SidePath);
					m_nSidePathAreasCount++;
				}
				m_ConnectedAreas.push_back(startArea);
			}
		}
	}
	///生成次要区域
	if (m_fSecondaryAreaRatio > 0.0f)
	{
		float ratio = (float)m_ConnectedAreas.size() / (float)m_Areas.size() + m_fSecondaryAreaRatio;
		if (ratio > 0.75f)
			ratio = 0.75f;
		int nConnected = (int)(m_Areas.size() * ratio);
		while (m_ConnectedAreas.size() < nConnected) {
			int rand = FMath::RandRange(0, (int)(m_ConnectedAreas.size()) - 1);
			Area* ca = static_cast<Area*>(m_ConnectedAreas[rand]);
			rand = FMath::RandRange(0, (int)(ca->getNeigbours().size()) - 1);
			Area* oa = static_cast<Area*>(ca->getNeigbours()[rand]);
			if (std::find(m_ConnectedAreas.begin(), m_ConnectedAreas.end(), oa) == m_ConnectedAreas.end()) {
				ca->connectArea(oa);
				if (oa->getAreaType() == EAreaTypeEnum::ATE_Unknown && oa->getAreaTypeMask() == EAreaTypeMaskEnum::ATME_Unknown)
				{
					oa->setAreaTypeMask(EAreaTypeMaskEnum::ATME_SecondaryArea);
					m_nSecondaryAreasCount++;
				}
				m_ConnectedAreas.push_back(oa);
			}
		}
	}

#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("m_Areas Count = %d"), (int)m_Areas.size());
	UE_LOG(LogTemp, Warning, TEXT("ConnectedAreas Count = %d"), (int)m_ConnectedAreas.size());
#endif // WITH_EDITOR
	return true;
}

bool DungeonGenerator::assignAreasType()
{
	for (Area* area : m_ConnectedAreas) {
		if (area)
		{
			if (area->getAreaType() == EAreaTypeEnum::ATE_Unknown && area->getConnectedAreas().size() == 1)
			{
			}
		}
	}

	return true;
}