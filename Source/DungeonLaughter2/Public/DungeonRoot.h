// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DungeonNodeComponent.h"
#include "DungeonRoot.generated.h"



UENUM(BlueprintType)
enum class ERandomGenerateType : uint8
{
	RGTE_CompletedRandom			UMETA(DisplayName = "CompletedRandom"),			///完全随机
	RGTE_ComplexityAscending		UMETA(DisplayName = "ComplexityAscending"),		///复杂度升序
	RGTE_ComplexityDescending		UMETA(DisplayName = "ComplexityDescending")		///复杂度降序
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "Whether generate dungeon tree randomly."))
	bool RandomGenerate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomGenerate", meta = (ShortTooltip = "Generate dungeon sort by complexity."))
	ERandomGenerateType RandomGenerateType;

	///UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomGenerate", meta = (ShortTooltip = "Generate dungeon sort by complexity."))
	///TRange<int> RandomGenerateType;

	// 蓝图逻辑调用部分 
	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	bool generateRandomDungeonTree(UDungeonNodeComponent* dungeonNode);

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
	void GenerateDungeon2dData();
private:
	UDungeonNodeComponent*	m_pDungeonRoot;
	UDungeonNodeComponent*	m_pCurrentDungeonNode;

	FString			m_strDungeonName; 
	FString			m_strBossDungeonName;
	FString			m_strBranchDungeonName;
	FString			m_strBranchBossDungeonName;
};
