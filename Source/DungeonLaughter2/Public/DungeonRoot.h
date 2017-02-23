// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DungeonNodeComponent.h"
#include "DungeonRoot.generated.h"

class ATerrainTile;
UENUM(BlueprintType)
enum class ERandomGenerateType : uint8
{
	RGTE_CompletedRandom			UMETA(DisplayName = "CompletedRandom"),			///完全随机
	RGTE_ComplexityAscending		UMETA(DisplayName = "ComplexityAscending")		///复杂度升序
};

UCLASS(Blueprintable)
class DUNGEONLAUGHTER2_API ADungeonRoot : public AActor
{
	GENERATED_BODY()
public:	

	// Sets default values for this actor's properties
	ADungeonRoot();

	virtual ~ADungeonRoot();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "Dungeon Type."))
	EDungeonType DungeonType;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Transient, Category = "Dungeon", meta = (ShortTooltip = "Max Dungeon Depth."))
	int MaxDungeonDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon", meta = (ClampMin = 1, ShortTooltip = "Per cell unit."))
	int CellUnit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomGenerate", meta = (ShortTooltip = "Whether generate dungeon tree randomly."))
	bool RandomGenerate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomGenerate", meta = (ClampMin = 1, ClampMax = 5, EditCondition = "RandomGenerate", ShortTooltip = "Random min dungeon depth."))
	int RandomMinDungeonDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomGenerate", meta = (ClampMin = 1, ClampMax = 5, EditCondition = "RandomGenerate", ShortTooltip = "Random max dungeon depth."))
	int RandomMaxDungeonDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomGenerate", meta = (EditCondition = "RandomGenerate", ShortTooltip = "Generate dungeon sort by complexity."))
	ERandomGenerateType RandomGenerateType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomGenerate", meta = (ClampMin = 32, ClampMax = 96, EditCondition = "RandomGenerate", ShortTooltip = "Random cell count X min value."))
	int RandomMinCellCountX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomGenerate", meta = (ClampMin = 32, ClampMax = 96, EditCondition = "RandomGenerate", ShortTooltip = "Random cell count X max value."))
	int RandomMaxCellCountX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomGenerate", meta = (ClampMin = 32, ClampMax = 96, EditCondition = "RandomGenerate", ShortTooltip = "Random cell count Y min value."))
	int RandomMinCellCountY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomGenerate", meta = (ClampMin = 32, ClampMax = 96, EditCondition = "RandomGenerate", ShortTooltip = "Random cell count Y max value."))
	int RandomMaxCellCountY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomGenerate", meta = (ClampMin = 7, ClampMax = 16, EditCondition = "RandomGenerate", ShortTooltip = "Random split area size min value."))
	int RandomMinSplitAreaSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomGenerate", meta = (ClampMin = 8, ClampMax = 16, EditCondition = "RandomGenerate", ShortTooltip = "Random split area size max value."))
	int RandomMaxSplitAreaSize;

	///TerrainTile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CellConfig", meta = (ShortTooltip = "Empty cell blueprints array."))
	TArray<UClass*> EmptyCellBlueprintsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CellConfig", meta = (ShortTooltip = "Standard floor blueprints array."))
	TArray<UClass*> StandardFloorBlueprintsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CellConfig", meta = (ShortTooltip = "Passage floor blueprints array."))
	TArray<UClass*> PassageFloorBlueprintsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CellConfig", meta = (ShortTooltip = "Tunnel floor blueprints array."))
	TArray<UClass*> TunnelFloorBlueprintsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CellConfig", meta = (ShortTooltip = "Standard Wall blueprints array."))
	TArray<UClass*> StandardWallBlueprintsArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CellConfig", meta = (ShortTooltip = "Passage Wall blueprints array."))
	TArray<UClass*> PassageWallBlueprintsArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CellConfig", meta = (ShortTooltip = "Entrance blueprints array."))
	TArray<UClass*> EntranceBlueprintsArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CellConfig", meta = (ShortTooltip = "Exit blueprints array."))
	TArray<UClass*> ExitBlueprintsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CellConfig", meta = (ShortTooltip = "Branch Exit blueprints array."))
	TArray<UClass*> BranchExitBlueprintsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CellConfig", meta = (ShortTooltip = "Standard door blueprints array."))
	TArray<UClass*> StandardDoorBlueprintsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CellConfig", meta = (ShortTooltip = "Locked door blueprints array."))
	TArray<UClass*> LockedDoorBlueprintsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CellConfig", meta = (ShortTooltip = "Hidden door blueprints array."))
	TArray<UClass*> HiddenDoorBlueprintsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CellConfig", meta = (ShortTooltip = "Mission locked door blueprints array."))
	TArray<UClass*> MissionLockedDoorBlueprintsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CellConfig", meta = (ShortTooltip = "Barricade blueprints array."))
	TArray<UClass*> BarricadeBlueprintsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CellConfig", meta = (ShortTooltip = "Boss treasury door blueprints array."))
	TArray<UClass*> BossTreasuryDoorBlueprintsArray;
	///
	// 蓝图逻辑调用部分 
	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	bool generateRandomDungeonTree();

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	bool initRootDungeonNode(UDungeonNodeComponent* dungeonNode);

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	bool addLeftDungeonNode(UDungeonNodeComponent* dungeonNode, UDungeonNodeComponent* leftDungeonNode);

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	bool addRightDungeonNode(UDungeonNodeComponent* dungeonNode, UDungeonNodeComponent* rightDungeonNode);

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	bool enterDungeon();

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	bool doDownstair(int& depth, bool goBranch = false);

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	bool doUpstair(int& depth);

	UDungeonNodeComponent* getRootDungeonNode() const { return m_pDungeonRoot; }
	UDungeonNodeComponent* getCurrentDungeonNode() const { return m_pCurrentDungeonNode; }

private:
	bool generateLeftRandomDungeonNode(UDungeonNodeComponent* parrent, int maxDepth);
	bool generateRightRandomDungeonNode(UDungeonNodeComponent* parrent, int maxDepth);
	void GenerateDungeon2dData();
	bool buildMap();
	ATerrainTile* findTerrianTileByCellType(ECellTypeEnum cellType);
	bool buildCell(const Cell& cell);
private:
	UDungeonNodeComponent*	m_pDungeonRoot;
	UDungeonNodeComponent*	m_pCurrentDungeonNode;

	FString			m_strDungeonName; 
	FString			m_strBossDungeonName;
	FString			m_strBranchDungeonName;
	FString			m_strBranchBossDungeonName;

	TArray<ATerrainTile*>	m_TerrainTileArray;
};
