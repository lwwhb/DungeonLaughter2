// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "DL2_HUD.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONLAUGHTER2_API ADL2_HUD : public AHUD
{
	GENERATED_BODY()

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

#if WITH_EDITOR
private:
	void EnableStep1();
	void EnableStep2();
	void EnableStep3();
	void EnableStep4();
	void EnableStep5();
	void EnableStep6();

	void DrawBackground();
	void DrawConnectAreas();
	void DrawSplitLines();
	void DrawEntranceAreaAndExitArea();
	void DrawStatistics();
private:
	FTimerHandle  m_TimerHandle;
	bool enableStep1;
	bool enableStep2;
	bool enableStep3;
	bool enableStep4;
	bool enableStep5;
	bool enableStep6;

	int unit;
#endif // WITH_EDITOR
	
};
