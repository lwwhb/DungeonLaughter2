// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "DL2_WorldDebugHUD.h"
#include "DungeonRoot.h"

ADL2_WorldDebugHUD::ADL2_WorldDebugHUD()
{
#if WITH_EDITOR
    m_pWorldRoot = nullptr;
#endif // WITH_EDITOR
}
void ADL2_WorldDebugHUD::BeginPlay()
{
    Super::BeginPlay();
    
#if WITH_EDITOR
    EnableSteps();
    UWorld* world = GetWorld();
    if (world)
    {
        int index = -1;
        if (!world->GetCurrentLevel()->Actors.FindItemByClass<AWorldRoot>(&m_pWorldRoot, &index))
            FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("This level need a AWorldRoot object.")));
    }
#endif // WITH_EDITOR
}
void ADL2_WorldDebugHUD::DrawHUD()
{
    Super::DrawHUD();
#if WITH_EDITOR
    if (!m_pWorldRoot)
        return;
    
#endif // WITH_EDITOR
}

#if WITH_EDITOR
void ADL2_WorldDebugHUD::EnableSteps()
{
    showSteps = true;
    enableStep1 = false;
    GetWorldTimerManager().SetTimer(m_TimerHandle, this, &ADL2_WorldDebugHUD::EnableStep1, 1.0f, false);
}
void ADL2_WorldDebugHUD::DisableSteps()
{
    showSteps = false;
}
void ADL2_WorldDebugHUD::EnableStep1()
{
    enableStep1 = true;
    //GetWorldTimerManager().SetTimer(m_TimerHandle, this, &ADL2_WorldDebugHUD::EnableStep2, 1.0f, false);
}
#endif // WITH_EDITOR
