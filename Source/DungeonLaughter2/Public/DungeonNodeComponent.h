// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "../Private/DungeonGenerator.h"
#include "DungeonNodeComponent.generated.h"

UENUM(BlueprintType)
enum class EVisitedType : uint8
{
	VTE_NO 		UMETA(DisplayName = "Visited_No"),			///没有进入过
	VTE_YES 	UMETA(DisplayName = "Visited_Yes"),			///进入过但没探索完成
	VTE_ALL		UMETA(DisplayName = "Visited_All")			///进入过并且探索完成
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONLAUGHTER2_API UDungeonNodeComponent : public UActorComponent
{
	friend class ADungeonRoot;
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Dungeon Style."))
	EDungeonStyle DungeonStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonNode", meta = (ClampMin = 32, ClampMax = 96, ShortTooltip = "X axis cells count."))
	int CellCountX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonNode", meta = (ClampMin = 32, ClampMax = 96, ShortTooltip = "Y axis cells count"))
	int CellCountY;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "DungeonNode", meta = (ShortTooltip = "Total cells count."))
	int CellTotalCount;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "DungeonNode", meta = (ShortTooltip = "Per cell unit."))
	int CellUnit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Wether dungeon node is boss dungeon node."))
	bool IsBossDungeonNode;

	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "DungeonNode", meta = (ClampMin = 7, ClampMax = 16, ShortTooltip = "Area split minimum size."))
	int MinSplitAreaSize;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "DungeonNode", meta = (ClampMin = 8, ClampMax = 16, ShortTooltip = "Area split maximum size."))
	int MaxSplitAreaSize;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "DungeonNode", meta = (ClampMin = 3, ClampMax = 1000, ShortTooltip = "Area minimum size. Value must less than half of MinSplitAreaSize, MinAreaSize < MinSplitAreaSize/2."))
	int MinAreaSize;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "DungeonNode", meta = (ClampMin = 4, ClampMax = 16, ShortTooltip = "Special Area minimum size. Value must more than MinAreaSize."))
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
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "DungeonNode", meta = (ClampMin = 0.0f, ClampMax = 0.5f, ShortTooltip = "Secondary Area gentertion ratio. Range 0.0~0.5"))
	float SecondaryAreaRatio;
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Need regenerate current dungeon."))
	bool  Regenerate;		///是否重新生成地牢

	UPROPERTY(BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Dungeon node name."))
	FString	DungeonNodeName;
	UPROPERTY(BlueprintReadWrite, Category = "DungeonNode", meta = (ShortTooltip = "Dungeon node depth."))
	int NodeDepth;

	// Sets default values for this component's properties
	UDungeonNodeComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UDungeonNodeComponent* getParentNode() const { return m_pParentNode; }
	UDungeonNodeComponent* getLeftNode() const { return m_pLeftNode; }
	UDungeonNodeComponent* getRightNode() const { return m_pRightNode; }
	EVisitedType getVisitedType() const { return m_VisitedType; }
private:
	UDungeonNodeComponent*	m_pParentNode;		///父节点
	UDungeonNodeComponent*	m_pLeftNode;		///main path
	UDungeonNodeComponent*	m_pRightNode;		///branch path
	EVisitedType			m_VisitedType;

	std::vector<Cell>	m_Map;
};
