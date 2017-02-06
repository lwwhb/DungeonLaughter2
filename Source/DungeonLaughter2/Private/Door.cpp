// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "Door.h"

Door::Door()
{
	m_Type = EDoorTypeEnum::DTE_Empty;
}

Door::~Door()
{
}

EDoorTypeEnum Door::getDoorType() const
{
	return m_Type;
}
void Door::setDoorType(EDoorTypeEnum type)
{
	m_Type = type;
}

FVector2D Door::getPos() const
{
	return m_Pos;
}
void Door::setPos(const FVector2D& pos)
{
	if (m_Pos != pos)
		m_Pos = pos;
}
