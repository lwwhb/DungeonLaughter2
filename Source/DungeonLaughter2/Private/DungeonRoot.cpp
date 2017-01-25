// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "DungeonRoot.h"
#include "../Private/DungeonGenerator.h"

// Sets default values
ADungeonRoot::ADungeonRoot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CellCountX = 48;
	CellCountY = 32;
	CellTotalCount = CellCountX * CellCountY;
	CellUnit = 100;

	MinSplitAreaSize = 7;
	MaxSplitAreaSize = 9;
	MinAreaSize = 3;
	MinSpecialAreaSize = 4;
	IsDoublePath = false;
	SecondaryAreaRatio = 0.0f;
}

// Called when the game starts or when spawned
void ADungeonRoot::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADungeonRoot::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ADungeonRoot::GenerateDungeon2dData()
{
	if (!DungeonGenerator::getInstance()->setGeneratorSetting(MinSplitAreaSize, MaxSplitAreaSize, MinAreaSize, MinSpecialAreaSize, IsDoublePath, SecondaryAreaRatio))
		return;
	if(!DungeonGenerator::getInstance()->generateDungeon(CellCountX, CellCountY))
		UE_LOG(LogTemp, Fatal, TEXT("Generate dungeon failed!"));
}