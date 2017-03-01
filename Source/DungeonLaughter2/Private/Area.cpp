// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "Area.h"
#include "DungeonGenerator.h"

Area::Area()
{
	m_AreaType = EAreaTypeEnum::ATE_Unknown;
	m_AreaTypeMask = EAreaTypeMaskEnum::ATME_Unknown;
	m_nWeight = 1;
}

Area::~Area()
{
}
bool Area::generate()
{
	bool ret = false;
	switch(m_AreaType)
	{
	case EAreaTypeEnum::ATE_Standard:
		ret = generateStandardArea();
		break;
	case EAreaTypeEnum::ATE_Passage:
		ret = generatePassageArea();
		break;
	case EAreaTypeEnum::ATE_Tunnel:
		ret = generateTunnelArea();
		break;
	case EAreaTypeEnum::ATE_Entrance:
		ret = generateEntranceArea();
		break;
	case EAreaTypeEnum::ATE_Exit:
		ret = generateExitArea();
		break;
	case EAreaTypeEnum::ATE_Branch:
		ret = generateBranchArea();
		break;
	case EAreaTypeEnum::ATE_Pivotal:
		ret = generateBranchArea();
		break;
	case EAreaTypeEnum::ATE_Special:
		ret = generateSpecialArea();
		break;
	case EAreaTypeEnum::ATE_Unusual:
		ret = generateUnusualArea();
	default:
		break;
	}
	return ret;
}
void Area::addNeigbourArea(Area* other)
{
	if (!other)
		return;
	FBox2D rect = getIntersectRect(other);
	if ((rect.GetSize().X == 0 && rect.GetSize().Y >= 3) || (rect.GetSize().Y == 0 && rect.GetSize().X >= 3))
	{
		m_Neigbours.push_back(other);
		other->m_Neigbours.push_back(this);
	}
}
void Area::connectArea(Area* other)
{
	if (!other)
		return;
	if (m_ConnectedAreas.find(other) == m_ConnectedAreas.end()) {
		m_ConnectedAreas[other] = nullptr;
		other->m_ConnectedAreas[this] = nullptr;
	}
}
FVector2D Area::getCenter()
{
	return FVector2D((int)(m_Rect.Min.X + m_Rect.Max.X) / 2 + (((int)(m_Rect.Max.X - m_Rect.Min.X) & 1) == 1 ? FMath::RandRange(0,1) : 0), (int)(m_Rect.Min.Y + m_Rect.Max.Y) / 2 + (((int)(m_Rect.Max.Y - m_Rect.Min.Y) & 1) == 1 ? FMath::RandRange(0, 1) : 0));
}
FBox2D Area::getRect() const
{
	return m_Rect;
}
void Area::setRect(const FBox2D& rect)
{
	m_Rect = rect;
}

EAreaTypeEnum Area::getAreaType() const
{
	return m_AreaType;
}
void Area::setAreaType(EAreaTypeEnum type)
{
	m_AreaType = type;
}
EAreaTypeMaskEnum Area::getAreaTypeMask() const
{
	return m_AreaTypeMask;
}
void Area::setAreaTypeMask(EAreaTypeMaskEnum mask)
{
	m_AreaTypeMask = mask;
}

FBox2D Area::getIntersectRect(Area* other)
{
	int minX = std::max(m_Rect.Min.X, other->getRect().Min.X);
	int minY = std::max(m_Rect.Min.Y, other->getRect().Min.Y);
	int maxX = std::min(m_Rect.Max.X, other->getRect().Max.X);
	int maxY = std::min(m_Rect.Max.Y, other->getRect().Max.Y);
	return FBox2D(FVector2D(minX, minY), FVector2D(maxX, maxY));
}

bool Area::generateStandardArea()
{
	generateCells(ECellTypeEnum::CTE_StandardWall);
	generateCells(1, ECellTypeEnum::CTE_StandardFloor);
	for (auto iter = m_ConnectedAreas.begin(); iter != m_ConnectedAreas.end(); iter++) {
		Door* door = iter->second;
		Area* other = iter->first;
		if (!other)
			return false;
		if(door)
			door->setDoorType(EDoorTypeEnum::DTE_Standard);
	}
	return true;
}
bool Area::generatePassageArea()
{
	int pasWidth = m_Rect.GetSize().X - 2;
	int pasHeight = m_Rect.GetSize().Y - 2;
	std::vector<int> joints;
	for (auto iter = m_ConnectedAreas.begin(); iter != m_ConnectedAreas.end(); iter++) {
		Door* door = iter->second;
		if (door)
			joints.push_back(xy2p(m_Rect, door->getPos()));
	}
	std::sort(joints.begin(), joints.end());

	int nJoints = (int)joints.size();
	int perimeter = pasWidth * 2 + pasHeight * 2;

	int start = 0;
	int maxD = joints[0] + perimeter - joints[nJoints - 1];
	for (int i = 1; i < nJoints; i++) {
		int d = joints[i] - joints[i - 1];
		if (d > maxD) {
			maxD = d;
			start = i;
		}
	}
	int end = (start + nJoints - 1) % nJoints;

	int p = joints[start];
	do {
		FVector2D xy = p2xy(m_Rect, p);
		DungeonGenerator::getInstance()->setCellType(xy.X, xy.Y, ECellTypeEnum::CTE_StandardFloor, m_AreaType, m_AreaTypeMask);
		p = (p + 1) % perimeter;
	} while (p != joints[end]);

	FVector2D  xy = p2xy(m_Rect, p);
	DungeonGenerator::getInstance()->setCellType(xy.X, xy.Y, ECellTypeEnum::CTE_StandardFloor, m_AreaType, m_AreaTypeMask);

	wrapCellByCellType(ECellTypeEnum::CTE_StandardFloor, ECellTypeEnum::CTE_PassageWall);

	for (auto iter = m_ConnectedAreas.begin(); iter != m_ConnectedAreas.end(); iter++) {
		Door* door = iter->second;
		Area* other = iter->first;
		if (!other)
			return false;
		if (door)
		{
			if (door->getDoorType() != EDoorTypeEnum::DTE_Empty)
				continue;
			door->setDoorType(EDoorTypeEnum::DTE_Passage);
		}
	}
	return true;
}
bool Area::generateTunnelArea()
{
	FVector2D c = getCenter();

	if (m_Rect.GetSize().X > m_Rect.GetSize().Y || (m_Rect.GetSize().X == m_Rect.GetSize().Y && FMath::RandRange(0, 1) == 0)) {

		int from = m_Rect.Max.X - 1;
		int to = m_Rect.Min.X + 1;

		for (auto iter = m_ConnectedAreas.begin(); iter != m_ConnectedAreas.end(); iter++) {
			Door* door = iter->second;
			if (door)
			{
				int step = door->getPos().Y < c.Y ? +1 : -1;

				if (door->getPos().X == m_Rect.Min.X) {

					from = m_Rect.Min.X + 1;
					for (int i = door->getPos().Y; i != c.Y; i += step) {
						DungeonGenerator::getInstance()->setCellType(from, i, ECellTypeEnum::CTE_TunnelFloor, m_AreaType, m_AreaTypeMask);
					}

				}
				else if (door->getPos().X == m_Rect.Max.X) {

					to = m_Rect.Max.X - 1;
					for (int i = door->getPos().Y; i != c.Y; i += step) {
						DungeonGenerator::getInstance()->setCellType(to, i, ECellTypeEnum::CTE_TunnelFloor, m_AreaType, m_AreaTypeMask);
					}

				}
				else {
					if (door->getPos().X < from) {
						from = door->getPos().X;
					}
					if (door->getPos().X > to) {
						to = door->getPos().X;
					}

					for (int i = door->getPos().Y + step; i != c.Y; i += step) {
						DungeonGenerator::getInstance()->setCellType(door->getPos().X, i, ECellTypeEnum::CTE_TunnelFloor, m_AreaType, m_AreaTypeMask);
					}
				}

			}
		}

		for (int i = from; i <= to; i++) {
			DungeonGenerator::getInstance()->setCellType(i, c.Y, ECellTypeEnum::CTE_TunnelFloor, m_AreaType, m_AreaTypeMask);
		}

	}
	else {

		int from = m_Rect.Max.Y - 1;
		int to = m_Rect.Min.Y + 1;
		for (auto iter = m_ConnectedAreas.begin(); iter != m_ConnectedAreas.end(); iter++) {
			Door* door = iter->second;
			if (door)
			{
				int step = door->getPos().X < c.X ? +1 : -1;

				if (door->getPos().Y == m_Rect.Min.Y) {

					from = m_Rect.Min.Y + 1;
					for (int i = door->getPos().X; i != c.X; i += step) {
						DungeonGenerator::getInstance()->setCellType(i, from, ECellTypeEnum::CTE_TunnelFloor, m_AreaType, m_AreaTypeMask);
					}

				}
				else if (door->getPos().Y == m_Rect.Max.Y) {

					to = m_Rect.Max.Y - 1;
					for (int i = door->getPos().X; i != c.X; i += step) {
						DungeonGenerator::getInstance()->setCellType(i, to, ECellTypeEnum::CTE_TunnelFloor, m_AreaType, m_AreaTypeMask);
					}

				}
				else {
					if (door->getPos().Y < from) {
						from = door->getPos().Y;
					}
					if (door->getPos().Y > to) {
						to = door->getPos().Y;
					}

					for (int i = door->getPos().X + step; i != c.X; i += step) {
						DungeonGenerator::getInstance()->setCellType(i, door->getPos().Y, ECellTypeEnum::CTE_TunnelFloor, m_AreaType, m_AreaTypeMask);
					}
				}

			}
		}

		for (int i = from; i <= to; i++) {
			DungeonGenerator::getInstance()->setCellType(c.X, i, ECellTypeEnum::CTE_TunnelFloor, m_AreaType, m_AreaTypeMask);
		}
	}

	for (auto iter = m_ConnectedAreas.begin(); iter != m_ConnectedAreas.end(); iter++) {
		Door* door = iter->second;
		Area* other = iter->first;
		if (!other)
			return false;
		if (door)
		{
			if (door->getDoorType() != EDoorTypeEnum::DTE_Empty)
				continue;
			door->setDoorType(EDoorTypeEnum::DTE_Tunnel);
		}
	}
	return true;
}
bool Area::generateEntranceArea()
{
	generateCells(ECellTypeEnum::CTE_StandardWall);
	generateCells(1, ECellTypeEnum::CTE_StandardFloor);
	for (auto iter = m_ConnectedAreas.begin(); iter != m_ConnectedAreas.end(); iter++) {
		Door* door = iter->second;
		if (door)
			door->setDoorType(EDoorTypeEnum::DTE_Standard);
	}
	setRandomTerrainTile(2, ECellTypeEnum::CTE_Entrance);
	return true;
}
bool Area::generateExitArea()
{
	generateCells(ECellTypeEnum::CTE_StandardWall);
	generateCells(1, ECellTypeEnum::CTE_StandardFloor);
	for (auto iter = m_ConnectedAreas.begin(); iter != m_ConnectedAreas.end(); iter++) {
		Door* door = iter->second;
		if (door)
			door->setDoorType(EDoorTypeEnum::DTE_Standard);
	}
	//setRandomTerrainTile(level, 2, TerrainTile::TT_ENTRANCE);
	return true;
}
bool Area::generateBranchArea()
{
	generateCells(ECellTypeEnum::CTE_StandardWall);
	generateCells(1, ECellTypeEnum::CTE_StandardFloor);
	for (auto iter = m_ConnectedAreas.begin(); iter != m_ConnectedAreas.end(); iter++) {
		Door* door = iter->second;
		if (door)
			door->setDoorType(EDoorTypeEnum::DTE_Standard);
	}
	//setRandomTerrainTile(level, 2, TerrainTile::TT_ENTRANCE);
	return true;
}
bool Area::generatePivotalArea()
{
	generateCells(ECellTypeEnum::CTE_StandardWall);
	generateCells(1, ECellTypeEnum::CTE_StandardFloor);
	for (auto iter = m_ConnectedAreas.begin(); iter != m_ConnectedAreas.end(); iter++) {
		Door* door = iter->second;
		if (door)
			door->setDoorType(EDoorTypeEnum::DTE_Standard);
	}
	//setRandomTerrainTile(level, 2, TerrainTile::TT_ENTRANCE);
	return true;
}
bool Area::generateSpecialArea()
{
	generateCells(ECellTypeEnum::CTE_StandardWall);
	generateCells(1, ECellTypeEnum::CTE_StandardFloor);
	for (auto iter = m_ConnectedAreas.begin(); iter != m_ConnectedAreas.end(); iter++) {
		Door* door = iter->second;
		if (door)
			door->setDoorType(EDoorTypeEnum::DTE_Standard);
	}
	//setRandomTerrainTile(level, 2, TerrainTile::TT_ENTRANCE);
	return true;
}
bool Area::generateUnusualArea()
{
	int x = m_Rect.Min.X;
	int y = m_Rect.Min.Y;
	int w = m_Rect.GetSize().X + 1;
	int h = m_Rect.GetSize().Y + 1;

	DungeonGenerator::getInstance()->setCellAreaType(x, y, w, h, EAreaTypeEnum::ATE_Unusual);
	return true;
}
void Area::generateCells(ECellTypeEnum cellType)
{
	int x = m_Rect.Min.X;
	int y = m_Rect.Min.Y;
	int w = m_Rect.GetSize().X + 1;
	int h = m_Rect.GetSize().Y + 1;

	DungeonGenerator::getInstance()->generateCells(x, y, w, h, cellType, m_AreaType, m_AreaTypeMask);
}
void Area::generateCells(int m, ECellTypeEnum cellType)
{
	int x = m_Rect.Min.X + m;
	int y = m_Rect.Min.Y + m;
	int w = m_Rect.GetSize().X + 1 - m * 2;
	int h = m_Rect.GetSize().Y + 1 - m * 2;

	DungeonGenerator::getInstance()->generateCells(x, y, w, h, cellType, m_AreaType, m_AreaTypeMask);
}
void Area::generateCells(const FBox2D& rect, ECellTypeEnum cellType)
{
	int x = m_Rect.Min.X + rect.Min.X;
	int y = m_Rect.Min.Y + rect.Min.Y;
	int w = m_Rect.GetSize().X + 1 - (rect.Max.X + rect.Min.X);
	int h = m_Rect.GetSize().Y + 1 - (rect.Max.Y + rect.Min.Y);

	DungeonGenerator::getInstance()->generateCells(x, y, w, h, cellType, m_AreaType, m_AreaTypeMask);
}
void Area::wrapCellByCellType(ECellTypeEnum cellType, ECellTypeEnum withCellType)
{
	int x = m_Rect.Min.X;
	int y = m_Rect.Min.Y;
	int w = m_Rect.GetSize().X + 1;
	int h = m_Rect.GetSize().Y + 1;

	DungeonGenerator::getInstance()->wrapCellByCellType(x, y, w, h, cellType, withCellType, m_AreaType, m_AreaTypeMask);
}
void Area::setRandomTerrainTile(int m, ECellTypeEnum cellType)
{
	int left = m_Rect.Min.X + m;
	int right = m_Rect.Max.X - m;
	int bottom = m_Rect.Min.Y + m;
	int top = m_Rect.Max.Y - m;
	int x = FMath::RandRange(FMath::Min(left, right), FMath::Max(left, right));
	int y = FMath::RandRange(FMath::Min(bottom, top), FMath::Max(bottom, top));

	DungeonGenerator::getInstance()->setCellType(x, y, cellType, m_AreaType, m_AreaTypeMask);
}
int Area::xy2p(const FBox2D& rect, const FVector2D& pos)
{
	int pasWidth = rect.GetSize().X - 2;
	int pasHeight = rect.GetSize().Y- 2;
	if (pos.Y == rect.Min.Y)
		return (pos.X - rect.Min.X - 1);
	else if (pos.X == rect.Max.X) 
		return (pos.Y - rect.Min.Y - 1) + pasWidth;
	else if (pos.Y == rect.Max.Y)
		return (rect.Max.X - pos.X - 1) + pasWidth + pasHeight;
	else 
	{
		if (pos.Y == rect.Min.Y + 1) 
			return 0;
		else 
			return (rect.Max.Y - pos.Y - 1) + pasWidth * 2 + pasHeight;
	}
}
FVector2D Area::p2xy(const FBox2D& rect, int p)
{
	int pasWidth = rect.GetSize().X - 2;
	int pasHeight = rect.GetSize().Y - 2;
	if (p < pasWidth) 
		return FVector2D(rect.Min.X + 1 + p, rect.Min.Y + 1);
	else if (p < pasWidth + pasHeight)
		return FVector2D(rect.Max.X - 1, rect.Min.Y + 1 + (p - pasWidth));
	else if (p < pasWidth * 2 + pasHeight)
		return FVector2D(rect.Max.X - 1 - (p - (pasWidth + pasHeight)), rect.Max.Y - 1);
	else
		return FVector2D(rect.Min.X + 1, rect.Max.Y - 1 - (p - (pasWidth * 2 + pasHeight)));
}

AreaInfo::AreaInfo()
{
	m_AreaType = EAreaTypeEnum::ATE_Unknown;
	m_AreaTypeMask = EAreaTypeMaskEnum::ATME_Unknown;
}
AreaInfo::~AreaInfo()
{
}
FBox2D AreaInfo::getRect() const
{
	return m_Rect;
}
void AreaInfo::setRect(const FBox2D& rect)
{
	m_Rect = rect;
}

EAreaTypeEnum AreaInfo::getAreaType() const
{
	return m_AreaType;
}
void AreaInfo::setAreaType(EAreaTypeEnum type)
{
	m_AreaType = type;
}
EAreaTypeMaskEnum AreaInfo::getAreaTypeMask() const
{
	return m_AreaTypeMask;
}
void AreaInfo::setAreaTypeMask(EAreaTypeMaskEnum mask)
{
	m_AreaTypeMask = mask;
}