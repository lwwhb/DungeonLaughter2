// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 地牢生成器
 */

#include <vector>
#include "PathGraphNode.h"
#include "Area.h"

UENUM(BlueprintType)
enum class EDungeonType : uint8
{
	DTE_Test 	UMETA(DisplayName = "Test"),
	DTE_Sewer 	UMETA(DisplayName = "Sewer")
};

UENUM(BlueprintType)
enum class EDungeonStyle : uint8
{
	DSE_Standard 	UMETA(DisplayName = "Standard"),
	DSE_Passage		UMETA(DisplayName = "Passage"),
	DSE_Tunnel		UMETA(DisplayName = "Tunnel")
};

class DungeonGenerator
{
public:
	DungeonGenerator();
	~DungeonGenerator();

	static DungeonGenerator* getInstance();
	static void releaseInstance();

	bool setGeneratorSetting(int width, int height, int cellUnit = 10,int minSplitAreaSize = 7, int maxSplitAreaSize = 9, int minAreaSize = 3, int minSpecialAreaSize = 4,
		bool doublePath = false, bool branchPath = false, bool loopBranchPath = false, bool multiLayerBranchPath = false, bool isImpasse = false, float secondaryAreaRatio = 0.0f,
		EDungeonStyle dungeonStyle = EDungeonStyle::DSE_Standard);
	bool generateDungeon(std::vector<Cell>& map);
	void generateCells(int x, int y, int width, int height, ECellTypeEnum cellType, EAreaTypeEnum areaType, EAreaTypeMaskEnum areaTypeMask, EDirectEnum direct = EDirectEnum::DE_Unknown);
	void setCellType(int posX, int posY, ECellTypeEnum cellType, EAreaTypeEnum areaType, EAreaTypeMaskEnum areaTypeMask, EDirectEnum direct = EDirectEnum::DE_Unknown);
	void setCellAreaType(int x, int y, int width, int height, EAreaTypeEnum areaType);
	void wrapCellByCellType(int x, int y, int width, int height, ECellTypeEnum cellType, ECellTypeEnum withCellType, EAreaTypeEnum areaType, EAreaTypeMaskEnum areaTypeMask, EDirectEnum direct = EDirectEnum::DE_Unknown);

	std::vector<int> getNeighbours4() { return{ -m_nWidth, +1, +m_nWidth, -1 }; }
	std::vector<int> getNeighbours5() { return{ 0, -m_nWidth, +1, +m_nWidth, -1 }; }
	std::vector<int> getNeighbours8() { return{ +1, -1, +m_nWidth, -m_nWidth, +1 + m_nWidth, +1 - m_nWidth, -1 + m_nWidth, -1 - m_nWidth }; }
	std::vector<int> getNeighbours9() { return{ 0, +1, -1, +m_nWidth, -m_nWidth, +1 + m_nWidth, +1 - m_nWidth, -1 + m_nWidth, -1 - m_nWidth }; }
	std::vector<int> getNeighbours13() { return{ 0, +1, -1, +m_nWidth, -m_nWidth, +1 + m_nWidth, +1 - m_nWidth, -1 + m_nWidth, -1 - m_nWidth, +2, -2, +m_nWidth * 2, -m_nWidth * 2 }; }
	std::vector<int> getNeighbours21() { return{ 0, +1, -1, +m_nWidth, -m_nWidth, +1 + m_nWidth, +1 - m_nWidth, -1 + m_nWidth, -1 - m_nWidth, +2, -2, +m_nWidth * 2, -m_nWidth * 2, +1 + m_nWidth * 2,-1 + m_nWidth * 2, +1 - m_nWidth * 2,-1 - m_nWidth * 2, -2 + m_nWidth, +2 + m_nWidth, +2 - m_nWidth, -2 - m_nWidth }; }
	std::vector<int> getNeighbours25() { return{ 0, +1, -1, +m_nWidth, -m_nWidth, +1 + m_nWidth, +1 - m_nWidth, -1 + m_nWidth, -1 - m_nWidth, +2, -2, +m_nWidth * 2, -m_nWidth * 2, +1 + m_nWidth * 2,-1 + m_nWidth * 2, +1 - m_nWidth * 2,-1 - m_nWidth * 2, -2 + m_nWidth, +2 + m_nWidth, +2 - m_nWidth, -2 - m_nWidth, +2 + m_nWidth * 2, +2 - m_nWidth * 2, -2 + m_nWidth * 2, -2 - m_nWidth * 2 }; }

	std::vector<Cell>& getMap();
	std::vector<PathGraphNode*>& getAreas();
	std::vector<Area*>& getConnectedAreas();
	std::vector<Area*>& getPivotalAreas();
	std::vector<Area*>& getSpecialAreas();
	std::vector<std::vector<Area*>>& getUnusualAreas();

	Area* getEntranceArea() const { return m_pAreaEntrance; }
	Area* getExitArea() const { return m_pAreaExit; }
	Area* getBranchArea() const { return m_pAreaBranch; }

	EDungeonStyle getDungeonStyle() const { return m_DungeonStyle; }

	int getWidth() const { return m_nWidth; }
	int getHeight() const { return m_nHeight; }
	int getCellUnit() const { return m_nCellUnit; }

	///获取统计数据
	int getMainPathAreasCount() const { return m_nMainPathAreasCount; }
	int getSidePathAreasCount() const { return m_nSidePathAreasCount; }
	int getBranchPathAreasCount() const { return m_nBranchPathAreasCount; }
	int getSecondaryAreasCount() const { return m_nSecondaryAreasCount; }
	int getPivotalAreasCount() const { return m_nPivotalAreasCount; }
	int getSpecialAreaCount() const { return m_nSpecialAreaCount; }
	int getUnusualAreaCount() const { return m_nUnusualAreaCount; }
	int getStandardAreaCount() const { return m_nStandardAreaCount; }
	int getPassageAreaCount() const { return m_nPassageAreaCount; }
	int getTunnelAreaCount() const { return m_nTunnelAreaCount; }

	int getStandardDoorCount() const { return m_nStandardDoorCount; }
	int getLockedDoorCount() const { return m_nLockedDoorCount; }
	int getHiddenDoorCount() const { return m_nHiddenDoorCount; }
	int getSpecialDoorCount() const { return m_nSpecialDoorCount; }
private:
	void reset();
	bool initAreas(const FBox2D& rect);
	void splitArea(const FBox2D& rect);
	bool connectArea();
	bool mergeSmallIntersectArea(Area* area, Area* other, bool generate = true);
	void addUnusualAreas(Area* area, Area* otherArea);
	bool assignAreasType();
	bool assignPivotalAreasType();
	bool assignSpecialAreasType();
	bool assignStandardAreasType();
	bool generateAreas();
	bool placeDoors(Area* area);
	bool generateDoors(Area* area);
	void generateUnusualAreas();
	bool generateTraps(Area* area);
private:
	std::vector<Cell>	m_Map;
	std::vector<PathGraphNode*>    m_Areas;
	Area*				m_pAreaEntrance;
	Area*				m_pAreaExit;
	Area*				m_pAreaBranch;
	bool				m_bDoublePath;
	bool				m_bBranchPath;
	bool				m_bLoopBranchPath;
	bool				m_bMultiLayerBranchPath;
	bool				m_bIsImpasse;
	float				m_fSecondaryAreaRatio;

	std::vector<Area*> m_ConnectedAreas;

	std::vector<Area*> m_MainPathAreas;
	std::vector<Area*> m_SidePathAreas;
	std::vector<Area*> m_BranchPathAreas;
	std::vector<Area*> m_SecondaryPathAreas;

	std::vector<Area*> m_PivotalAreas;
	std::vector<Area*> m_SpecialAreas;
	std::vector<std::vector<Area*>> m_UnusualAreas;

	std::vector<Door*> m_Doors;

	EDungeonStyle	m_DungeonStyle;

	int		m_nWidth;			///地牢X方向大小
	int		m_nHeight;			///地牢Y方向大小
	int		m_nCellUnit;		///地牢Cell单位大小

	int		m_nMinSplitAreaSize; ///最小分割区域大小
	int		m_nMaxSplitAreaSize; ///最大分割区域大小
	int		m_nMinAreaSize;		 ///最小区域大小
	int		m_nMinSpecialAreaSize;	///最小特殊区域大小

	///统计数据
	int		m_nMainPathAreasCount;	///主路径区域数
	int		m_nSidePathAreasCount;  ///辅路径区域数
	int		m_nBranchPathAreasCount;///分支路径区域数
	int		m_nSecondaryAreasCount;	///次要区域数

	int		m_nPivotalAreasCount;	///关键区域计数器
	int		m_nSpecialAreaCount;	///特殊区域计数器
	int		m_nUnusualAreaCount;	///独特区域计数器
	int		m_nStandardAreaCount;	///标准区域计数器
	int		m_nPassageAreaCount;	///通道区域计数器
	int		m_nTunnelAreaCount;		///隧道区域计数器

	int		m_nStandardDoorCount;	///标准门计数器
	int		m_nLockedDoorCount;		///锁住的门计数器
	int		m_nHiddenDoorCount;		///隐藏门计数器
	int		m_nSpecialDoorCount;	///特殊门计数器（包括任务门，路障和宝藏室）
};
