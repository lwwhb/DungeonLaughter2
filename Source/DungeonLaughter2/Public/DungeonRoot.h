// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DungeonNodeComponent.h"
#include "DungeonRoot.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "Max Dungeon Depth."))
	int MaxDungeonDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dungeon", meta = (ShortTooltip = "Wether generate boss in max depth."))
	bool GenerateBoss;

	// 蓝图逻辑调用部分 
	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	void GenerateDungeon2dData();

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	bool initRootDungeonNode(UDungeonNodeComponent* dungeonNode);

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	bool addLeftDungeonNode(UDungeonNodeComponent* dungeonNode, UDungeonNodeComponent* leftDungeonNode);

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	bool addRightDungeonNode(UDungeonNodeComponent* dungeonNode, UDungeonNodeComponent* rightDungeonNode);

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	bool doDownstair(int& depth, bool goBranch = false);

	UFUNCTION(BlueprintCallable, Category = "Dungeon")
	bool doUpstair(int& depth);
private:
	UDungeonNodeComponent*	m_pDungeonRoot;
	UDungeonNodeComponent*	m_pCurrentDungeonNode;

	FString			m_strDungeonName; 
	FString			m_strBossDungeonName;
	FString			m_strBranchDungeonName;
	FString			m_strBranchBossDungeonName;
};
