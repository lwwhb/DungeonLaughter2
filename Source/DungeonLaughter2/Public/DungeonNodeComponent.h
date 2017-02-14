// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "../Private/DungeonGenerator.h"
#include "DungeonNodeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONLAUGHTER2_API UDungeonNodeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Dungeon Style."))
	EDungeonStyle DungeonStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "X axis cells count."))
	int CellCountX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Y axis cells count"))
	int CellCountY;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "DungeonNode", meta = (ShortTooltip = "Total cells count."))
	int CellTotalCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Per cell unit."))
	int CellUnit;

	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Area split minimum size."))
	int MinSplitAreaSize;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Area split maximum size."))
	int MaxSplitAreaSize;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Area minimum size. Value must less than half of MinSplitAreaSize, MinAreaSize < MinSplitAreaSize/2."))
	int MinAreaSize;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Special Area minimum size. Value must more than MinAreaSize."))
	int MinSpecialAreaSize;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Wether generate two path from entrance to exit."))
	bool UseDoublePath;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Wether generate branch exit."))
	bool UseBranchExit;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Wether generate looped branch path."))
	bool UseLoopBranchPath;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Wether generate multilayer branch path."))
	bool MultiLayerBranchPath;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Wether the dungeon is impasse."))
	bool IsImpasse;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Secondary Area gentertion ratio. Range 0.0~0.5"))
	float SecondaryAreaRatio;

	UPROPERTY(BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Dungeon node name."))
	FString			m_strDungeonNodeName;
	UPROPERTY(BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Dungeon node depth."))
	int				m_nNodeDepth;
	UPROPERTY(BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Wether dungeon node is boss dungeon node."))
	bool			m_bIsBossDungeonNode;

	UDungeonNodeComponent*	m_pParentNode;		///¸¸½Úµã
	UDungeonNodeComponent*	m_pLeftNode;		///main path
	UDungeonNodeComponent*	m_pRightNode;		///branch path

	// Sets default values for this component's properties
	UDungeonNodeComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	
};
