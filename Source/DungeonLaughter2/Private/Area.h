// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
#include "PathGraphNode.h"
#include "Door.h"
#include <unordered_map>

UENUM(BlueprintType)
enum class EAreaTypeEnum : uint8
{
	ATE_Unknown 	UMETA(DisplayName = "Unknown"),
	ATE_Standard 	UMETA(DisplayName = "Standard"),
	ATE_Entrance	UMETA(DisplayName = "Entrance"),
	ATE_Exit		UMETA(DisplayName = "Exit"),
	ATE_Branch_Exit UMETA(DisplayName = "Branch_Exit")
};
UENUM(BlueprintType)
enum class EAreaTypeMaskEnum : uint8
{
	ATME_Unknown 			UMETA(DisplayName = "Unknown"),
	ATME_Entrance			UMETA(DisplayName = "Entrance"),
	ATME_Exit				UMETA(DisplayName = "Exit"),
	ATME_Branch_Exit		UMETA(DisplayName = "Branch_Exit"),
	ATME_MainPath 			UMETA(DisplayName = "MainPath"),
	ATME_SidePath			UMETA(DisplayName = "SidePath"),
	ATME_BranchPath			UMETA(DisplayName = "BranchPath"),
	ATME_SecondaryArea		UMETA(DisplayName = "SecondaryArea")
};
class Area : public PathGraphNode
{
public:
	Area();
	virtual ~Area();

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
	FBox2D       m_Rect;

	EAreaTypeEnum  m_AreaType;
	EAreaTypeMaskEnum  m_AreaTypeMask;

	std::unordered_map<Area*, Door*>    m_ConnectedAreas;
};
