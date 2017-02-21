// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "DL2_GameMode.h"
#include "DungeonRoot.h"
#include "../Private/DungeonGenerator.h"

void ADL2_GameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();
	if (world)
	{
		int index = -1;
		if (!world->GetCurrentLevel()->Actors.FindItemByClass<ADungeonRoot>(&m_pDungeonRoot, &index))
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("This level need a ADungeonRoot object.")));
	}
}

void ADL2_GameMode::expandDebugMap()
{
	int unit = DungeonGenerator::getInstance()->getDisplayCellUnit();
	unit += 1;
	DungeonGenerator::getInstance()->setDisplayCellUnit(FMath::Min(unit, 15));
}

void ADL2_GameMode::reduceDebugMap()
{
	int unit = DungeonGenerator::getInstance()->getDisplayCellUnit();
	unit -= 1;
	DungeonGenerator::getInstance()->setDisplayCellUnit(FMath::Max(unit, 5));
}

void ADL2_GameMode::goUpstair()
{
	if (m_pDungeonRoot)
	{
		int currentDepth;
		if (!m_pDungeonRoot->doUpstair(currentDepth))
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Have reached the first floor!")));
	}
}

void ADL2_GameMode::goDownstair()
{
	if (m_pDungeonRoot)
	{
		int currentDepth;
		if (!m_pDungeonRoot->doDownstair(currentDepth))
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Have reached the last floor!")));
	}
}
void ADL2_GameMode::goBranchDownstair()
{
	if (m_pDungeonRoot)
	{
		int currentDepth;
		if (!m_pDungeonRoot->doDownstair(currentDepth, true))
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Have reached the last branch floor or there is no next branch floor!")));
	}
}