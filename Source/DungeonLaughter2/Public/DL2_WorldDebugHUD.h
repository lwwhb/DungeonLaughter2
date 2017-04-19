// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "DL2_WorldDebugHUD.generated.h"

class AWorldRoot;
/**
 * 
 */
UCLASS()
class DUNGEONLAUGHTER2_API ADL2_WorldDebugHUD : public AHUD
{
	GENERATED_BODY()
	
    ADL2_WorldDebugHUD();
    
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    
    virtual void DrawHUD() override;
	
#if WITH_EDITOR
public:
    void EnableSteps();
    void DisableSteps();
private:
    void EnableStep1();
private:
    FTimerHandle    m_TimerHandle;
    bool            enableStep1;
    
    bool            showSteps;
    AWorldRoot*     m_pWorldRoot;
#endif // WITH_EDITOR
};
