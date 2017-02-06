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

	void generateCells(ECellTypeEnum cellType);
	void generateCells(int m, ECellTypeEnum cellType);
	void generateCells(const FBox2D& rect, ECellTypeEnum cellType);
private:
	FBox2D       m_Rect;

	EAreaTypeEnum  m_AreaType;
	EAreaTypeMaskEnum  m_AreaTypeMask;

	std::unordered_map<Area*, Door*>    m_ConnectedAreas;
};
