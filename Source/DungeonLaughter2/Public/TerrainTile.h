// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CellActor.h"
#include "../Private/Cell.h"
#include "TerrainTile.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONLAUGHTER2_API ATerrainTile : public ACellActor
{
	GENERATED_BODY()

public:
	ATerrainTile();
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "TerrainTile", meta = (ShortTooltip = "Cell type."))
	ECellTypeEnum CellType;

	bool addInstance(const FTransform& transform);
};
