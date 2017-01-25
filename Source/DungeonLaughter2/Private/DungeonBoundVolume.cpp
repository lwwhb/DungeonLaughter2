// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "DungeonBoundVolume.h"
#include "Components/BrushComponent.h"

ADungeonBoundVolume::ADungeonBoundVolume(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetBrushComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	GetBrushComponent()->Mobility = EComponentMobility::Static;

	BrushColor = FColor(200, 200, 200, 255);

	bColored = true;
}

/*void ADungeonBoundVolume::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
}

void ADungeonBoundVolume::PostUnregisterAllComponents()
{
	Super::PostUnregisterAllComponents();
}

#if WITH_EDITOR
void ADungeonBoundVolume::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif // WITH_EDITOR*/

