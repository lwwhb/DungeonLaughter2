// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */

UENUM(BlueprintType)
enum class EDoorTypeEnum : uint8
{
	DTE_Empty 		UMETA(DisplayName = "Empty"),
	DTE_Standard 	UMETA(DisplayName = "Standard")
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
