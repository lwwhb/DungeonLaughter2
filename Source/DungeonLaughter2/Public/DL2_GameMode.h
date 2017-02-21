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

public:
	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	void expandDebugMap();

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	void reduceDebugMap();

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	void goUpstair();

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	void goDownstair();

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	void goBranchDownstair();
private:
	ADungeonRoot*	m_pDungeonRoot;
};
