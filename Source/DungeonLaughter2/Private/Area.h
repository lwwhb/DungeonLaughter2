// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
#include "PathGraphNode.h"
#include "Door.h"
#include "Cell.h"
#include <unordered_map>

class Area : public PathGraphNode
{
public:
	Area();
	~Area();

	bool generate();

	void addNeigbourArea(Area* other);
	void connectArea(Area* other);

	FVector2D getCenter();

	FBox2D getRect() const;
	void setRect(const FBox2D& rect);

	EAreaTypeEnum getAreaType() const;
	void setAreaType(EAreaTypeEnum type);

	EAreaTypeMaskEnum getAreaTypeMask() const;
	void setAreaTypeMask(EAreaTypeMaskEnum mask);

	FBox2D getIntersectRect(Area* other);

	std::unordered_map<Area*, Door*>& getConnectedAreas() { return m_ConnectedAreas; }
private:
	bool generateStandardArea();
	bool generatePassageArea();
	bool generateTunnelArea();
	bool generateEntranceArea();
	bool generateExitArea();
	bool generateBranchArea();
	bool generatePivotalArea();
	bool generateSpecialArea();
	bool generateUnusualArea();

	void generateCells(ECellTypeEnum cellType);
	void generateCells(int m, ECellTypeEnum cellType);
	void generateCells(const FBox2D& rect, ECellTypeEnum cellType);
	void wrapCellByCellType(ECellTypeEnum cellType, ECellTypeEnum withCellType);
	void setRandomTerrainTile(int m, ECellTypeEnum cellType);

	int xy2p(const FBox2D& rect, const FVector2D& pos);
	FVector2D p2xy(const FBox2D& rect, int p);
private:
	FBox2D       m_Rect;

	EAreaTypeEnum  m_AreaType;
	EAreaTypeMaskEnum  m_AreaTypeMask;

	std::unordered_map<Area*, Door*>    m_ConnectedAreas;
};
