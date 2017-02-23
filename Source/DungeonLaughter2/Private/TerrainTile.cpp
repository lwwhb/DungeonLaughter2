// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "TerrainTile.h"


ATerrainTile::ATerrainTile()
{
	CellType = ECellTypeEnum::CTE_EmptyCell;
}
bool ATerrainTile::addInstance(const FTransform& transform)
{
	if (RootComponent->GetAttachChildren().Num() <= 0)
		return false;
	UInstancedStaticMeshComponent* instancedStaticMeshComponent = Cast<UInstancedStaticMeshComponent>(RootComponent->GetAttachChildren()[0]);
	if (!instancedStaticMeshComponent)
		return false;
	instancedStaticMeshComponent->AddInstanceWorldSpace(transform);
	return true;
}