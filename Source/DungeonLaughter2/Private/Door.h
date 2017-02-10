// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

UENUM(BlueprintType)
enum class EDoorTypeEnum : uint8
{
	DTE_Empty 				UMETA(DisplayName = "Empty"),
	DTE_Standard 			UMETA(DisplayName = "Standard"),
	DTE_Passage				UMETA(DisplayName = "Passage"),
	DTE_Tunnel				UMETA(DisplayName = "Tunnel"),
	DTE_Locked				UMETA(DisplayName = "Locked"),
	DTE_Hidden				UMETA(DisplayName = "Hidden"),
	DTE_MissionLocked		UMETA(DisplayName = "MissionLocked"),
	DTE_Barricade			UMETA(DisplayName = "Barricade"),
	DTE_BossTreasury		UMETA(DisplayName = "BossTreasury")
};

class Door
{
public:
	Door();
	~Door();

	EDoorTypeEnum getDoorType() const;
	void setDoorType(EDoorTypeEnum type);

	FVector2D getPos() const;
	void setPos(const FVector2D& pos);
private:
	EDoorTypeEnum           m_Type;
	FVector2D				m_Pos;
};
