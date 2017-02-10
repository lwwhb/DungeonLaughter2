// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "DungeonGenerator.h"
#include "AlisaMethod.h"
#include <algorithm>

DungeonGenerator* g_pDungeonGeneratorInstance = nullptr;
DungeonGenerator* DungeonGenerator::getInstance()
{
	if (g_pDungeonGeneratorInstance == nullptr)
		g_pDungeonGeneratorInstance = new DungeonGenerator();

	return g_pDungeonGeneratorInstance;
}
DungeonGenerator::DungeonGenerator()
{
	reset();
}

DungeonGenerator::~DungeonGenerator()
{
}
bool DungeonGenerator::setGeneratorSetting(int width, int height, int cellUnit, int minSplitAreaSize, int maxSplitAreaSize, int minAreaSize, int minSpecialAreaSize,
	bool doublePath, bool branchPath, bool loopBranchPath, bool multiLayerBranchPath, bool isImpasse, float secondaryAreaRatio, EDungeonStyle dungeonStyle)
{
	reset();
	if (width > 96 || width < 32)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("CellCountX value must between 32 to 96.")));
		return false;
	}
	if (height > 96 || height < 32)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("CellCountY value must between 32 to 96.")));
		return false;
	}
	if (maxSplitAreaSize > 16 || maxSplitAreaSize < 8)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("MaxSplitAreaSize value must between 8 to 16.")));
		return false;
	}
	if (minSplitAreaSize > 16 || minSplitAreaSize < 4)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("MaxSplitAreaSize value must between 4 to 16.")));
		return false;
	}
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
	if (loopBranchPath && !branchPath)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("If it doesn't generate branch path, you can't loop the branch path.")));
		return false;
	}
	if (multiLayerBranchPath && !branchPath)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("If it doesn't generate branch path, you can't generate multiLayer branch path.")));
		return false;
	}
	if (isImpasse && branchPath)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("If it's impasse dungeon, it can't generate branch path.")));
		return false;
	}
	if (secondaryAreaRatio < 0.0f || secondaryAreaRatio > 0.5f)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("SecondaryAreaRatio value must between 0.0f~0.5f")));
		return false;
	}
	m_nWidth = width + 1;
	m_nHeight = height + 1;
	m_nCellUnit = cellUnit;
	m_nMinSplitAreaSize = minSplitAreaSize;
	m_nMaxSplitAreaSize = maxSplitAreaSize;
	m_nMinAreaSize = minAreaSize;				///最小区域大小
	m_nMinSpecialAreaSize = minSpecialAreaSize; ///最小特殊区域大小
	m_bDoublePath = doublePath;
	m_bBranchPath = branchPath;
	m_bLoopBranchPath = loopBranchPath;
	m_bMultiLayerBranchPath = multiLayerBranchPath;
	m_bIsImpasse = isImpasse;
	m_fSecondaryAreaRatio = secondaryAreaRatio;
	m_DungeonStyle = dungeonStyle;

	m_Map.resize(m_nWidth*m_nHeight);
	for (int i = 0; i < m_nHeight; i++)
	{
		for (int j = 0; j < m_nWidth; j++)
		{
			int index = i*m_nHeight + j;
			m_Map[index].setIndexX(j);
			m_Map[index].setIndexY(i);
		}
	}
	return true;
}
bool DungeonGenerator::generateDungeon()
{
	if (m_Map.empty())
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Please call setGeneratorSetting function first!")));
		return false;
	}
	if (!initAreas(FBox2D(FVector2D(0, 0), FVector2D(m_nWidth-1, m_nHeight-1))))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Init areas failed!"));
		return false;
	}
	if (!connectArea())
	{
		UE_LOG(LogTemp, Fatal, TEXT("Connect areas failed!"));
		return false;
	}
	if (!assignAreasType())
	{
		UE_LOG(LogTemp, Fatal, TEXT("Assign areas type failed!"));
		return false;
	}
	if (!generateAreas())
	{
		UE_LOG(LogTemp, Fatal, TEXT("Generate areas failed!"));
		return false;
	}
	return true;
}
void DungeonGenerator::generateCells(int x, int y, int width, int height, ECellTypeEnum cellType, EAreaTypeEnum areaType, EAreaTypeMaskEnum areaTypeMask, EDirectEnum direct)
{
	int pos = y * m_nWidth + x;
	for (int i = y; i < y + height; i++, pos += m_nWidth) {
		for (int j = pos, k = x; j<(pos + width); j++, k++) {
			m_Map[j].setCellType(cellType);
			m_Map[j].setAreaType(areaType);
			m_Map[j].setAreaTypeMask(areaTypeMask);
			//m_Map[j].m_Flag = assignTerrainTileFlag(cellType);
			m_Map[j].setDirect(direct);
		}
	}
}
void DungeonGenerator::setCellType(int posX, int posY, ECellTypeEnum cellType, EAreaTypeEnum areaType, EAreaTypeMaskEnum areaTypeMask, EDirectEnum direct)
{
	int index = posX + posY * m_nWidth;
	m_Map[index].setCellType(cellType);
	m_Map[index].setAreaType(areaType);
	m_Map[index].setAreaTypeMask(areaTypeMask);
	//m_Map[index].m_Flag = assignTerrainTileFlag(cellType);
	m_Map[index].setDirect(direct);
}
void DungeonGenerator::setCellAreaType(int x, int y, int width, int height, EAreaTypeEnum areaType)
{
	int pos = x + y * m_nWidth;
	for (int i = y; i < y + height; i++, pos += m_nWidth) {
		for (int j = pos; j < (pos + width); j++) {
			m_Map[j].setAreaType(areaType);
		}
	}
}
void DungeonGenerator::wrapCellByCellType(int x, int y, int width, int height, ECellTypeEnum cellType, ECellTypeEnum withCellType, EAreaTypeEnum areaType, EAreaTypeMaskEnum areaTypeMask, EDirectEnum direct)
{
	int pos = y * m_nWidth + x;
	for (int i = y; i < y + height; i++, pos += m_nWidth) {
		for (int j = pos; j<(pos + width); j++) {
			if (m_Map[j].getCellType() == cellType)
			{
				for (int n : getNeighbours8()) {
					int cell = j + n;
					if (m_Map[cell].getCellType() == ECellTypeEnum::CTE_Empty)
					{
						m_Map[cell].setCellType(withCellType);
						m_Map[cell].setAreaType(areaType);
						m_Map[cell].setAreaTypeMask(areaTypeMask);
						//m_Map[cell].m_Flag = assignTerrainTileFlag(withCellType);
						m_Map[cell].setDirect(direct);
					}
				}
			}
		}
	}
}
std::vector<Cell>& DungeonGenerator::getMap()
{
	return m_Map;
}
std::vector<PathGraphNode*>& DungeonGenerator::getAreas()
{
	return m_Areas;
}
std::vector<Area*>& DungeonGenerator::getConnectedAreas()
{
	return m_ConnectedAreas;
}
std::vector<Area*>& DungeonGenerator::getPivotalAreas()
{
	return m_PivotalAreas;
}
std::vector<Area*>& DungeonGenerator::getSpecialAreas()
{
	return m_SpecialAreas;
}
std::vector<std::vector<Area*>>& DungeonGenerator::getUnusualAreas()
{
	return m_UnusualAreas;
}
void DungeonGenerator::reset()
{
	m_pAreaEntrance = nullptr;
	m_pAreaExit = nullptr;
	m_pAreaBranch = nullptr;

	if (!m_MainPathAreas.empty())
		m_MainPathAreas.clear();
	if (!m_SidePathAreas.empty())
		m_SidePathAreas.clear();
	if (!m_BranchPathAreas.empty())
		m_BranchPathAreas.clear();
	if (!m_SecondaryPathAreas.empty())
		m_SecondaryPathAreas.clear();
	if (!m_PivotalAreas.empty())
		m_PivotalAreas.clear();
	if (!m_SpecialAreas.empty())
		m_SpecialAreas.clear();
	if (!m_UnusualAreas.empty())
	{
		for (size_t i = 0; i < m_UnusualAreas.size(); i++)
		{
			if (!m_UnusualAreas[i].empty())
				m_UnusualAreas[i].clear();
		}
		m_UnusualAreas.clear();
	}
	if (!m_ConnectedAreas.empty())
		m_ConnectedAreas.clear();
	if (!m_Doors.empty())
	{
		std::vector<Door*>::iterator iter;
		for (iter = m_Doors.begin(); iter != m_Doors.end(); iter++)
		{
			if (*iter)
			{
				delete *iter;
				*iter = nullptr;
			}
		}
		m_Doors.clear();
	}
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
	m_Map.clear();
	m_bDoublePath = false;
	m_bBranchPath = false;
	m_bLoopBranchPath = false;
	m_bMultiLayerBranchPath = false;
	m_bIsImpasse = false;
	m_fSecondaryAreaRatio = 0.0f;

	m_nWidth = 0;
	m_nHeight = 0;
	m_nCellUnit = 10;

	m_nMinSplitAreaSize = 0;
	m_nMaxSplitAreaSize = 0;
	m_nMinAreaSize = 0;
	m_nMinSpecialAreaSize = 0;

	m_DungeonStyle = EDungeonStyle::DSE_Standard;

	m_nMainPathAreasCount = 0;
	m_nSidePathAreasCount = 0;
	m_nBranchPathAreasCount = 0;
	m_nSecondaryAreasCount = 0;
	m_nPivotalAreasCount = 0;
	m_nSpecialAreaCount = 0;
	m_nUnusualAreaCount = 0;
	m_nStandardAreaCount = 0;
	m_nPassageAreaCount = 0;
	m_nTunnelAreaCount = 0;

	m_nStandardDoorCount	= 0;	///标准门计数器
	m_nLockedDoorCount		= 0;	///锁住的门计数器
	m_nHiddenDoorCount		= 0;	///隐藏门计数器
	m_nSpecialDoorCount		= 0;	///特殊门计数器（包括任务门，路障和宝藏室）
}
bool DungeonGenerator::initAreas(const FBox2D& rect)
{	
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
		UE_LOG(LogTemp, Warning, TEXT("generate entrance and exit retry time = %d"), retry++);
#endif // WITH_EDITOR
	} while (distance < minDistance);

	if (m_bBranchPath)
	{
		int distance1 = 0;
		int distance2 = 0;
		int retry1 = 0;
		do
		{
			do
			{
				int rand = FMath::RandRange(0, (int)(m_Areas.size()) - 1);
				m_pAreaBranch = static_cast<Area*>(m_Areas[rand]);
			} while (m_pAreaBranch->getRect().GetSize().X < 4 || m_pAreaBranch->getRect().GetSize().Y < 4);

			PathGraph::buildDistanceMap(m_Areas, m_pAreaBranch);

			distance1 = m_pAreaEntrance->getDistance();
			distance2 = m_pAreaExit->getDistance();
#if WITH_EDITOR
			UE_LOG(LogTemp, Warning, TEXT("generate branch exit retry time = %d"), retry1++);
#endif // WITH_EDITOR
		} while (distance1 < (int)sqrt(minDistance) || distance2 < (int)sqrt(minDistance));
	}

	if (m_pAreaEntrance)
	{
		m_pAreaEntrance->setAreaType(EAreaTypeEnum::ATE_Entrance);
		m_pAreaEntrance->setAreaTypeMask(EAreaTypeMaskEnum::ATME_Entrance);
	}
	if (m_pAreaExit)
	{
		m_pAreaExit->setAreaType(EAreaTypeEnum::ATE_Exit);
		m_pAreaExit->setAreaTypeMask(EAreaTypeMaskEnum::ATME_Exit);
	}
	if (m_pAreaBranch)
	{
		m_pAreaBranch->setAreaType(EAreaTypeEnum::ATE_Branch);
		if(m_bMultiLayerBranchPath)
			m_pAreaBranch->setAreaTypeMask(EAreaTypeMaskEnum::ATME_Branch_Exit);
		else
			m_pAreaBranch->setAreaTypeMask(EAreaTypeMaskEnum::ATME_Branch_Mission);
	}

	///生成第一条路径
	if(m_ConnectedAreas.empty())
		m_ConnectedAreas.push_back(m_pAreaEntrance);
	PathGraph::buildDistanceMap(m_Areas, m_pAreaExit);

	std::vector<PathGraphNode*> path = PathGraph::buildPath(m_Areas, m_pAreaEntrance, m_pAreaExit);
	Area* startArea = m_pAreaEntrance;
	for (PathGraphNode* node : path) {
		Area* next = static_cast<Area*>(node);
		if (next)
		{
			if (std::find(m_ConnectedAreas.begin(), m_ConnectedAreas.end(), next) == m_ConnectedAreas.end()) {
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
				if (std::find(m_ConnectedAreas.begin(), m_ConnectedAreas.end(), next) == m_ConnectedAreas.end()) {
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

		Area* last = m_ConnectedAreas[m_ConnectedAreas.size() - 1];
		if (last)
		{
			for (size_t i = 0; i < last->getNeigbours().size(); i++)
			{
				Area* cn = static_cast<Area*>(last->getNeigbours()[i]);
				if (cn && (cn->getAreaTypeMask() == EAreaTypeMaskEnum::ATME_MainPath || cn == m_pAreaExit))
					last->connectArea(cn);
			}
		}
	}
	///生成分支路径
	if (m_bBranchPath)
	{
		///生成第一条分支路径
		PathGraph::buildDistanceMap(m_Areas, m_pAreaBranch);

		std::vector<PathGraphNode*> branchPath = PathGraph::buildPath(m_Areas, m_pAreaEntrance, m_pAreaBranch);
		Area* startArea1 = m_pAreaEntrance;
		for (PathGraphNode* node : branchPath) {
			Area* next = static_cast<Area*>(node);
			if (next)
			{
				if (std::find(m_ConnectedAreas.begin(), m_ConnectedAreas.end(), next) == m_ConnectedAreas.end()) {
					startArea1->connectArea(next);
					startArea1 = next;
					if (startArea1->getAreaType() == EAreaTypeEnum::ATE_Unknown && startArea1->getAreaTypeMask() == EAreaTypeMaskEnum::ATME_Unknown)
					{
						startArea1->setAreaTypeMask(EAreaTypeMaskEnum::ATME_BranchPath);
						m_nBranchPathAreasCount++;
					}
					m_ConnectedAreas.push_back(startArea1);
				}
			}
		}
		PathGraph::setWeight(branchPath, m_pAreaBranch->getDistance());
		///生成回路分支路径
		if (m_bLoopBranchPath)
		{
			PathGraph::buildDistanceMap(m_Areas, m_pAreaExit);
			branchPath = PathGraph::buildPath(m_Areas, m_pAreaBranch, m_pAreaExit);

			startArea1 = m_pAreaBranch;
			for (PathGraphNode* node : branchPath) {
				Area* next = static_cast<Area*>(node);
				if (next)
				{
					if (std::find(m_ConnectedAreas.begin(), m_ConnectedAreas.end(), next) == m_ConnectedAreas.end()) {
						startArea1->connectArea(next);
						startArea1 = next;
						if (startArea1->getAreaType() == EAreaTypeEnum::ATE_Unknown && startArea1->getAreaTypeMask() == EAreaTypeMaskEnum::ATME_Unknown)
						{
							startArea1->setAreaTypeMask(EAreaTypeMaskEnum::ATME_BranchPath);
							m_nBranchPathAreasCount++;
						}
						m_ConnectedAreas.push_back(startArea1);
					}
				}
			}
			Area* last = m_ConnectedAreas[m_ConnectedAreas.size() - 1];
			if (last)
			{
				for (size_t i = 0; i < last->getNeigbours().size(); i++)
				{
					Area* cn = static_cast<Area*>(last->getNeigbours()[i]);
					if (cn && (cn->getAreaTypeMask() == EAreaTypeMaskEnum::ATME_MainPath || cn->getAreaTypeMask() == EAreaTypeMaskEnum::ATME_SidePath || cn == m_pAreaExit))
						last->connectArea(cn);
				}
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

bool DungeonGenerator::mergeSmallIntersectArea(Area* area, Area* other, bool generate)
{
	if (!area || !other)
		return false;
	if (area->getAreaType() != EAreaTypeEnum::ATE_Standard || other->getAreaType() != EAreaTypeEnum::ATE_Standard)
		return false;

	FBox2D rect = area->getIntersectRect(other);
	if (rect.Min.X == rect.Max.X)
	{
		if (rect.Max.Y - rect.Min.Y < m_nMinAreaSize)
			return false;

		if (rect.GetSize().Y == FMath::Max(area->getRect().GetSize().Y, other->getRect().GetSize().Y)) 
			return false;

		if (area->getRect().GetSize().X + other->getRect().GetSize().X > m_nMaxSplitAreaSize) 
			return false;

		if (generate)
		{
			rect.Min.X += 0;
			rect.Min.Y += 1;
			rect.Max.X += 1;
			rect.Max.Y += 0;
			generateCells(rect.Min.X, rect.Min.Y, 1, rect.GetSize().Y, ECellTypeEnum::CTE_StandardFloor, area->getAreaType(), area->getAreaTypeMask());
		}
	}
	else if(rect.Min.Y == rect.Max.Y)
	{

		if (rect.Max.X - rect.Min.X < m_nMinAreaSize)
			return false;

		if (rect.GetSize().X == FMath::Max(area->getRect().GetSize().X, other->getRect().GetSize().X))
			return false;

		if (area->getRect().GetSize().Y + other->getRect().GetSize().Y > m_nMaxSplitAreaSize) 
			return false;

		if (generate)
		{
			rect.Min.X += 1;
			rect.Min.Y += 0;
			rect.Max.X += 0;
			rect.Max.Y += 1;
			generateCells(rect.Min.X, rect.Min.Y, rect.GetSize().X, 1, ECellTypeEnum::CTE_StandardFloor, area->getAreaType(), area->getAreaTypeMask());
		}
	}
	return true;
}
void DungeonGenerator::addUnusualAreas(Area* area, Area* otherArea)
{
	if (m_UnusualAreas.empty())
	{
		std::vector<Area*> unusualArea;
		unusualArea.push_back(area);
		m_nStandardAreaCount--;
		unusualArea.push_back(otherArea);
		m_nStandardAreaCount--;
		m_UnusualAreas.push_back(unusualArea);
		m_nUnusualAreaCount++;
	}
	else
	{
		bool find = false;
		for (size_t i = 0; i < m_UnusualAreas.size(); i++)
		{
			if (std::find(m_UnusualAreas[i].begin(), m_UnusualAreas[i].end(), area) != m_UnusualAreas[i].end())
			{
				find = true;
				if (std::find(m_UnusualAreas[i].begin(), m_UnusualAreas[i].end(), otherArea) == m_UnusualAreas[i].end())
				{
					m_UnusualAreas[i].push_back(otherArea);
					m_nStandardAreaCount--;
					break;
				}
			}
			else if (std::find(m_UnusualAreas[i].begin(), m_UnusualAreas[i].end(), otherArea) != m_UnusualAreas[i].end())
			{
				find = true;
				if (std::find(m_UnusualAreas[i].begin(), m_UnusualAreas[i].end(), area) == m_UnusualAreas[i].end())
				{
					m_UnusualAreas[i].push_back(area);
					m_nStandardAreaCount--;
					break;
				}
			}
			else
				continue;
		}

		if (!find)
		{
			std::vector<Area*> unusualArea;
			unusualArea.push_back(area);
			m_nStandardAreaCount--;
			unusualArea.push_back(otherArea);
			m_nStandardAreaCount--;
			m_UnusualAreas.push_back(unusualArea);
			m_nUnusualAreaCount++;
		}
	}
}
bool compareFunc(Area*& first, Area*& second)
{ 
	return first->getRect().GetArea() > second->getRect().GetArea();
}
bool DungeonGenerator::assignAreasType()
{
	///分离数组排序
	for (Area* area : m_ConnectedAreas) {
		if (area)
		{
			if (area->getAreaTypeMask() == EAreaTypeMaskEnum::ATME_MainPath)
				m_MainPathAreas.push_back(area);
			else if (area->getAreaTypeMask() == EAreaTypeMaskEnum::ATME_SidePath)
				m_SidePathAreas.push_back(area);
			else if (area->getAreaTypeMask() == EAreaTypeMaskEnum::ATME_BranchPath)
				m_BranchPathAreas.push_back(area);
			else if (area->getAreaTypeMask() == EAreaTypeMaskEnum::ATME_SecondaryArea)
				m_SecondaryPathAreas.push_back(area);
		}
	}
	std::sort(m_MainPathAreas.begin(), m_MainPathAreas.end(), compareFunc);
	std::sort(m_SidePathAreas.begin(), m_SidePathAreas.end(), compareFunc);
	std::sort(m_BranchPathAreas.begin(), m_BranchPathAreas.end(), compareFunc);
	std::sort(m_SecondaryPathAreas.begin(), m_SecondaryPathAreas.end(), compareFunc);
	if (m_MainPathAreas.size() > 0)
	{
		int count = (int)sqrt(m_MainPathAreas.size()+2);
		for (int i = 0; i < count; i++)
		{
			m_MainPathAreas[i]->setAreaType(EAreaTypeEnum::ATE_Pivotal);
			m_PivotalAreas.push_back(m_MainPathAreas[i]);
			m_nPivotalAreasCount++;
		}
	}
	if (m_SidePathAreas.size() > 0)
	{
		int count = (int)sqrt(m_SidePathAreas.size()+2);
		for (int i = 0; i < count; i++)
		{
			m_SidePathAreas[i]->setAreaType(EAreaTypeEnum::ATE_Pivotal);
			m_PivotalAreas.push_back(m_SidePathAreas[i]);
			m_nPivotalAreasCount++;
		}
	}
	if (m_BranchPathAreas.size() > 0)
	{
		int count = (int)sqrt(m_BranchPathAreas.size()+2);
		for (int i = 0; i < count; i++)
		{
			m_BranchPathAreas[i]->setAreaType(EAreaTypeEnum::ATE_Pivotal);
			m_PivotalAreas.push_back(m_BranchPathAreas[i]);
			m_nPivotalAreasCount++;
		}
	}
	if (m_SecondaryPathAreas.size() > 0)
	{
		for (int i = 0; i < m_SecondaryPathAreas.size(); i++)
		{
			Area* area = m_SecondaryPathAreas[i];
			if (area && area->getAreaType() == EAreaTypeEnum::ATE_Unknown && area->getConnectedAreas().size() == 1)
			{
				if (area->getRect().GetSize().X >= m_nMinSpecialAreaSize && area->getRect().GetSize().Y >= m_nMinSpecialAreaSize && FMath::RandRange(0, m_nSpecialAreaCount*m_nSpecialAreaCount + 1) == 0)
				{
					area->setAreaType(EAreaTypeEnum::ATE_Special);
					m_SpecialAreas.push_back(area);
					m_nSpecialAreaCount++;
				}
				else if (FMath::RandRange(0,1) == 0)
				{
					std::vector<PathGraphNode*> neigbours;
					for (PathGraphNode* n : area->getNeigbours()) {
						Area* areaNeigbour = static_cast<Area*>(n);
						if (areaNeigbour)
						{
							if (areaNeigbour->getConnectedAreas().find(areaNeigbour) == areaNeigbour->getConnectedAreas().end() && areaNeigbour->getAreaType() == EAreaTypeEnum::ATE_Special)
							{
								neigbours.push_back(areaNeigbour);
							}
						}
					}
					if (neigbours.size() > 1) {
						int rand = FMath::RandRange(0, (int)(neigbours.size()) - 1);
						area->connectArea(static_cast<Area*>(neigbours[rand]));
					}
				}
			}
		}
	}
	if (!assignPivotalAreasType())
	{
		UE_LOG(LogTemp, Fatal, TEXT("Assign pivotal areas type failed!"));
		return false;
	}
	if (!assignSpecialAreasType())
	{
		UE_LOG(LogTemp, Fatal, TEXT("Assign special areas type failed!"));
		return false;
	}
	if (!assignStandardAreasType())
	{
		UE_LOG(LogTemp, Fatal, TEXT("Assign standard areas type failed!"));
		return false;
	}
	return true;
}
bool DungeonGenerator::assignPivotalAreasType()
{
	return true;
}
bool DungeonGenerator::assignSpecialAreasType()
{
	return true;
}
bool DungeonGenerator::assignStandardAreasType()
{
	for (Area* area : m_ConnectedAreas)
	{
		if (!area)
			return false;
		if (area->getAreaType() == EAreaTypeEnum::ATE_Unknown)
		{
			int connections = (int)area->getConnectedAreas().size();
			if (connections == 0)
			{
				UE_LOG(LogTemp, Fatal, TEXT("Connected areas count is zero!"));
				return false;
			}
			else if (FMath::RandRange(0, connections * connections - 1) == 0)
			{
				area->setAreaType(EAreaTypeEnum::ATE_Standard);
				m_nStandardAreaCount++;
			}
			else
			{
				switch (m_DungeonStyle)
				{
				case EDungeonStyle::DSE_Standard:
					area->setAreaType(EAreaTypeEnum::ATE_Standard);
					m_nStandardAreaCount++;
					break;
				case EDungeonStyle::DSE_Passage:
					area->setAreaType(EAreaTypeEnum::ATE_Passage);
					m_nPassageAreaCount++;
					break;
				case EDungeonStyle::DSE_Tunnel:
					area->setAreaType(EAreaTypeEnum::ATE_Tunnel);
					m_nTunnelAreaCount++;
					break;
				default:
					break;
				}
			}
		}
	}

	///保证标准区域至少4个
	int retry = 0;
	while (m_nStandardAreaCount < 4) {
		int rand = FMath::RandRange(0, (int)(m_Areas.size()) - 1);
		Area* area = static_cast<Area*>(m_Areas[rand]);
		if (area != nullptr) {
			if (area->getAreaType() == EAreaTypeEnum::ATE_Passage)
			{
				area->setAreaType(EAreaTypeEnum::ATE_Standard);
				m_nTunnelAreaCount--;
				if (m_nTunnelAreaCount < 0)
					m_nTunnelAreaCount = 0;
				m_nStandardAreaCount++;
			}
			else if (area->getAreaType() == EAreaTypeEnum::ATE_Tunnel)
			{
				area->setAreaType(EAreaTypeEnum::ATE_Standard);
				m_nPassageAreaCount--;
				if (m_nPassageAreaCount < 0)
					m_nPassageAreaCount = 0;
				m_nStandardAreaCount++;
			}
			else
			{
				if (retry++ > 60) {
					break;
				}
			}
		}
	}
	return true;
}

bool DungeonGenerator::generateAreas()
{
	for (Area* area : m_ConnectedAreas)
	{
		if (!area)
			return false;
		if (area->getAreaType() != EAreaTypeEnum::ATE_Unknown)
		{
			if (!placeDoors(area))
			{
				UE_LOG(LogTemp, Fatal, TEXT("Place doors failed!"));
				return false;
			}
			if (!area->generate())
			{
				UE_LOG(LogTemp, Fatal, TEXT("Area generate failed!"));
				return false;
			}
		}
	}
	for (Area* area : m_ConnectedAreas) {
		if (!area)
			return false;
		if (!generateDoors(area))
		{
			UE_LOG(LogTemp, Fatal, TEXT("Generate doors failed!"));
			return false;
		}
	}
	generateUnusualAreas();

	for (Area* area : m_ConnectedAreas) {
		if (!area)
			return false;
		if (!generateTraps(area))
		{
			UE_LOG(LogTemp, Fatal, TEXT("Generate traps failed!"));
			return false;
		}
	}
	
	for (size_t i = 0; i < m_Doors.size(); i++)
	{
		Door* door = m_Doors[i];
		if (door)
		{
			if (door->getDoorType() == EDoorTypeEnum::DTE_Standard)
				m_nStandardDoorCount++;
			else if (door->getDoorType() == EDoorTypeEnum::DTE_Locked)
				m_nLockedDoorCount++;
			else if (door->getDoorType() == EDoorTypeEnum::DTE_Hidden)
				m_nHiddenDoorCount++;
			else if (door->getDoorType() == EDoorTypeEnum::DTE_MissionLocked || door->getDoorType() == EDoorTypeEnum::DTE_Barricade || door->getDoorType() == EDoorTypeEnum::DTE_BossTreasury)
				m_nSpecialDoorCount++;
		}
	}
	return true;
}
bool DungeonGenerator::placeDoors(Area* area)
{
	if (!area)
		return false;
	for (auto iter = area->getConnectedAreas().begin(); iter != area->getConnectedAreas().end(); iter++) {
		Area* areaOri = iter->first;
		Door* door = iter->second;
		if (!areaOri)
			return false;
		if (mergeSmallIntersectArea(area, areaOri, false))
			continue;
		if (door == nullptr)
		{
			FBox2D rect = area->getIntersectRect(areaOri);
			if (rect.GetSize().X == 0)
			{
				door = new(std::nothrow) Door();
				if (!door)
					return false;
				door->setPos(FVector2D(rect.Min.X, FMath::RandRange((int)rect.Min.Y + 1, (int)rect.Max.Y - 1)));
			}
			else if (rect.GetSize().Y == 0)
			{
				door = new(std::nothrow) Door();
				if (!door)
					return false;
				door->setPos(FVector2D(FMath::RandRange((int)rect.Min.X + 1, (int)rect.Max.X - 1), rect.Min.Y));
			}
			else
				continue;
			if (door)
			{
				m_Doors.push_back(door);
				area->getConnectedAreas()[areaOri] = door;
				areaOri->getConnectedAreas()[area] = door;
			}
		}
	}
	return true;
}
bool DungeonGenerator::generateDoors(Area* area)
{
	if (!area)
		return false;
	for (auto iter = area->getConnectedAreas().begin(); iter != area->getConnectedAreas().end(); iter++) {
		Area* areaOri = iter->first;
		if (mergeSmallIntersectArea(area, areaOri))
		{
			addUnusualAreas(area, areaOri);
			continue;
		}
		Door* door = iter->second;
		if (door)
		{
			EDirectEnum dir = EDirectEnum::DE_Unknown;
			if (door->getPos().X == area->getRect().Min.X)
				dir = EDirectEnum::DE_Left;
			else if (door->getPos().X == area->getRect().Max.X)
				dir = EDirectEnum::DE_Right;
			else
			{
				if (door->getPos().Y == area->getRect().Min.Y)
					dir = EDirectEnum::DE_Backward;
				else if (door->getPos().Y == area->getRect().Max.Y)
					dir = EDirectEnum::DE_Forward;
			}
			switch (door->getDoorType()) {
			case EDoorTypeEnum::DTE_Empty:
				setCellType(door->getPos().X, door->getPos().Y, ECellTypeEnum::CTE_StandardFloor, area->getAreaType(), area->getAreaTypeMask(), dir);
				break;
			case EDoorTypeEnum::DTE_Passage:
				setCellType(door->getPos().X, door->getPos().Y, ECellTypeEnum::CTE_PassageFloor, area->getAreaType(), area->getAreaTypeMask(), dir);
				break;
			case EDoorTypeEnum::DTE_Tunnel:
				setCellType(door->getPos().X, door->getPos().Y, ECellTypeEnum::CTE_TunnelFloor, area->getAreaType(), area->getAreaTypeMask(), dir);
				break;
			case EDoorTypeEnum::DTE_Standard:
				setCellType(door->getPos().X, door->getPos().Y, ECellTypeEnum::CTE_StandardDoor, area->getAreaType(), area->getAreaTypeMask(), dir);
				break;
			case EDoorTypeEnum::DTE_Locked:
				setCellType(door->getPos().X, door->getPos().Y, ECellTypeEnum::CTE_LockedDoor, area->getAreaType(), area->getAreaTypeMask(), dir);
				break;
			case EDoorTypeEnum::DTE_Hidden:
				setCellType(door->getPos().X, door->getPos().Y, ECellTypeEnum::CTE_HiddenDoor, area->getAreaType(), area->getAreaTypeMask(), dir);
				break;
			case EDoorTypeEnum::DTE_MissionLocked:
				setCellType(door->getPos().X, door->getPos().Y, ECellTypeEnum::CTE_MissionLockedDoor, area->getAreaType(), area->getAreaTypeMask(), dir);
				break;
			case EDoorTypeEnum::DTE_Barricade:
				setCellType(door->getPos().X, door->getPos().Y, ECellTypeEnum::CTE_Barricade, area->getAreaType(), area->getAreaTypeMask(), dir);
				break;
			case EDoorTypeEnum::DTE_BossTreasury:
				setCellType(door->getPos().X, door->getPos().Y, ECellTypeEnum::CTE_BossTreasuryDoor, area->getAreaType(), area->getAreaTypeMask(), dir);
				break;
			default:
				break;
			}
		}
	}
	return true;
}
void DungeonGenerator::generateUnusualAreas()
{
	for (size_t i = 0; i < m_UnusualAreas.size(); i++)
	{
		for (size_t j = 0; j < m_UnusualAreas[i].size(); j++)
		{
			m_UnusualAreas[i][j]->setAreaType(EAreaTypeEnum::ATE_Unusual);
			m_UnusualAreas[i][j]->generate();
		}
	}
}
bool DungeonGenerator::generateTraps(Area* area)
{
	if (!area)
		return false;
	return true;
}