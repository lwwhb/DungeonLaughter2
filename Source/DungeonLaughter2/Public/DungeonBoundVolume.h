// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Volume.h"
#include "DungeonBoundVolume.generated.h"

/**
 * 用于定义程序化地牢生成范围
 */
UCLASS(Blueprintable)
class DUNGEONLAUGHTER2_API ADungeonBoundVolume : public AVolume
{
	GENERATED_UCLASS_BODY()


	/*//~ Begin AActor Interface
	virtual void PostRegisterAllComponents() override;
	virtual void PostUnregisterAllComponents() override;
	//~ End AActor Interface
#if WITH_EDITOR
	//~ Begin UObject Interface
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface
#endif // WITH_EDITOR*/
	
	
	
};
