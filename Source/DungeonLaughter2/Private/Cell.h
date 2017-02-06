// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

UENUM(BlueprintType)
enum class ECellTypeEnum : uint8
{
	CTE_Empty 		UMETA(DisplayName = "Empty"),
	CTE_Floor 		UMETA(DisplayName = "Floor"),
	CTE_Wall		UMETA(DisplayName = "Wall")
};


UENUM(BlueprintType)
enum class EAreaTypeEnum : uint8
{
	ATE_Unknown 	UMETA(DisplayName = "Unknown"),
	ATE_Standard 	UMETA(DisplayName = "Standard"),
	ATE_Passage		UMETA(DisplayName = "Passage"),
	ATE_Tunnel		UMETA(DisplayName = "Tunnel"),
	ATE_Entrance	UMETA(DisplayName = "Entrance"),
	ATE_Exit		UMETA(DisplayName = "Exit"),
	ATE_Branch		UMETA(DisplayName = "Branch"),
	ATE_Pivotal		UMETA(DisplayName = "Pivotal"),
	ATE_Special		UMETA(DisplayName = "Special")
};
UENUM(BlueprintType)
enum class EAreaTypeMaskEnum : uint8
{
	ATME_Unknown 			UMETA(DisplayName = "Unknown"),
	ATME_Entrance			UMETA(DisplayName = "Entrance"),
	ATME_Exit				UMETA(DisplayName = "Exit"),
	ATME_Branch_Exit		UMETA(DisplayName = "Branch_Exit"),
	ATME_Branch_Mission		UMETA(DisplayName = "Branch_Mission"),
	ATME_MainPath 			UMETA(DisplayName = "MainPath"),
	ATME_SidePath			UMETA(DisplayName = "SidePath"),
	ATME_BranchPath			UMETA(DisplayName = "BranchPath"),
	ATME_SecondaryArea		UMETA(DisplayName = "SecondaryArea")
};

UENUM(BlueprintType)
enum class EDirectEnum : uint8
{
	DE_Forward		UMETA(DisplayName = "Forward"),
	DE_Left			UMETA(DisplayName = "Left"),
	DE_Right		UMETA(DisplayName = "Right"),
	DE_Backward		UMETA(DisplayName = "Backward"),
	DE_Unknown		UMETA(DisplayName = "Unknown")
};
class Cell
{
public:
	Cell();
	~Cell();

	int getIndexX() const { return m_nIndexX; }
	void setIndexX(int indexX) { m_nIndexX = indexX; }

	int getIndexY() const { return m_nIndexY; }
	void setIndexY(int indexY) { m_nIndexY = indexY; }

	ECellTypeEnum getCellType() const { return m_Type; }
	void setCellType(ECellTypeEnum type) { m_Type = type; }

	EAreaTypeEnum getAreaType() const { return m_AreaType; }
	void setAreaType(EAreaTypeEnum type) { m_AreaType = type; }

	EAreaTypeMaskEnum getAreaTypeMask() const { return m_AreaTypeMask; }
	void setAreaTypeMask(EAreaTypeMaskEnum mask) { m_AreaTypeMask = mask; }

	EDirectEnum getDirect() const { return m_Direct; }
	void setDirect(EDirectEnum direct) { m_Direct = direct; }
private:
	int						m_nIndexX;
	int						m_nIndexY;
	ECellTypeEnum			m_Type;
	EAreaTypeEnum			m_AreaType;
	EAreaTypeMaskEnum		m_AreaTypeMask;
	EDirectEnum				m_Direct;
};
