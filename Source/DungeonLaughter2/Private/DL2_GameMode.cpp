// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "DL2_GameMode.h"
#include "DungeonRoot.h"
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