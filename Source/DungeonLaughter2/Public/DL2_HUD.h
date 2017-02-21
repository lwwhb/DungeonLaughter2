// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "DL2_HUD.generated.h"

class UDungeonNodeComponent;
class ADungeonRoot;
/**
 * 
 */
UCLASS()
class DUNGEONLAUGHTER2_API ADL2_HUD : public AHUD
{
	GENERATED_BODY()

	ADL2_HUD();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void DrawHUD() override;

#if WITH_EDITOR
public:
	void EnableSteps();
	void DisableSteps();
private:
	void EnableStep1();
	void EnableStep2();
	void EnableStep3();
	void EnableStep4();
	void EnableStep5();
	void EnableStep6();
	void EnableStep7();

	void DrawBackground();
	void DrawConnectAreas();
	void DrawSplitLines();
	void DrawEntranceAreaAndExitArea();
	void DrawFinalMap();
	void DrawStatistics();

	void DrawCircle(const FVector2D& Center, const FColor& Color, int32 NumSides, float Radius);
	void DrawDungeonNode(UDungeonNodeComponent* node, const FVector2D& Center);
	void DrawDungeonTreeInfo();
private:
	FTimerHandle  m_TimerHandle;
	bool enableStep1;
	bool enableStep2;
	bool enableStep3;
	bool enableStep4;
	bool enableStep5;
	bool enableStep6;
	bool enableStep7;

	int unit;

	bool showSteps;

	ADungeonRoot*	m_pDungeonRoot;
#endif // WITH_EDITOR
	
};
