// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "DL2_GameMode.generated.h"

class ADungeonRoot;
/**
 * 
 */
UCLASS()
class DUNGEONLAUGHTER2_API ADL2_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

private:
	ADungeonRoot*	m_pDungeonRoot;
};
