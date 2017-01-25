// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 地牢生成器
 */

#include <vector>
#include "PathGraphNode.h"
class Area;

UENUM(BlueprintType)
enum class EDungeonStyle : uint8
{
	DSE_Unknown 	UMETA(DisplayName = "Unknown"),
	DSE_Standard 	UMETA(DisplayName = "Standard"),
};

class DungeonGenerator
{
public:
	DungeonGenerator();
	~DungeonGenerator();

	static DungeonGenerator* getInstance();

	bool setGeneratorSetting(int minSplitAreaSize = 7, int maxSplitAreaSize = 9, int minAreaSize = 3, int minSpecialAreaSize = 4,
		bool doublePath = false, float secondaryAreaRatio = 0.0f);
	bool generateDungeon(int Width, int Height);

	std::vector<PathGraphNode*>& getAreas();
	std::vector<Area*>& getConnectedAreas();

	Area* getEntranceArea() const { return m_pAreaEntrance; }
	Area* getExitArea() const { return m_pAreaExit; }

	int getWidth() const { return m_nWidth; }
	int getHeight() const { return m_nHeight; }

	///获取统计数据
	int getMainPathAreasCount() const { return m_nMainPathAreasCount; }
	int getSidePathAreasCount() const { return m_nSidePathAreasCount; }
	int getSecondaryAreasCount() const { return m_nSecondaryAreasCount; }
protected:
	virtual bool initAreas(const FBox2D& rect);
	virtual void splitArea(const FBox2D& rect);
	virtual bool connectArea();
	virtual bool assignAreasType();
private:
	std::vector<PathGraphNode*>    m_Areas;
	Area*				m_pAreaEntrance;
	Area*				m_pAreaExit;
	bool				m_bDoublePath;
	float				m_fSecondaryAreaRatio;

	std::vector<Area*> m_ConnectedAreas;

	int		m_nWidth;			///地牢X方向大小
	int		m_nHeight;			///地牢Y方向大小

	int		m_nMinSplitAreaSize; ///最小分割区域大小
	int		m_nMaxSplitAreaSize; ///最大分割区域大小
	int		m_nMinAreaSize;		 ///最小区域大小
	int		m_nMinSpecialAreaSize;	///最小特殊区域大小


	///统计数据
	int		m_nMainPathAreasCount;	///主路径区域数
	int		m_nSidePathAreasCount;  ///辅路径区域数
	int		m_nSecondaryAreasCount;	///次要区域数

	int		m_nSpecialAreaCount;	///特殊区域计数器
};
