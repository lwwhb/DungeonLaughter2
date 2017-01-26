// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "../Private/DungeonGenerator.h"
#include "DungeonRoot.generated.h"

UCLASS(Blueprintable)
class DUNGEONLAUGHTER2_API ADungeonRoot : public AActor
{
	GENERATED_BODY()
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "Dungeon Style."))
	EDungeonStyle DungeonStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "X axis cells count."))
	int CellCountX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "Y axis cells count"))
	int CellCountY;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Dungeon", meta = (ShortTooltip = "Total cells count."))
	int CellTotalCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "Per cell unit."))
	int CellUnit;

	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "Area split minimum size."))
	int MinSplitAreaSize;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "Area split maximum size."))
	int MaxSplitAreaSize;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "Area minimum size. Value must less than half of MinSplitAreaSize, MinAreaSize < MinSplitAreaSize/2."))
	int MinAreaSize;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "Special Area minimum size. Value must more than MinAreaSize."))
	int MinSpecialAreaSize;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "Wether generate two path from entrance to exit."))
	bool UseDoublePath;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "Wether generate branch exit."))
	bool UseBranchExit;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "Wether generate looped branch path."))
	bool UseLoopBranchPath;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "Wether the dungeon is impasse."))
	bool IsImpasse;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "Secondary Area gentertion ratio. Range 0.0~0.5"))
	float SecondaryAreaRatio;

	// Sets default values for this actor's properties
	ADungeonRoot();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	// 蓝图逻辑调用部分 
	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	void GenerateDungeon2dData();
};
