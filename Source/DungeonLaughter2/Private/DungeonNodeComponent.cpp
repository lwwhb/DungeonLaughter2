// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "DungeonNodeComponent.h"


// Sets default values for this component's properties
UDungeonNodeComponent::UDungeonNodeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	DungeonStyle = EDungeonStyle::DSE_Standard;
	CellCountX = 48;
	CellCountY = 32;
	CellTotalCount = CellCountX * CellCountY;
	CellUnit = 10;

	MinSplitAreaSize = 7;
	MaxSplitAreaSize = 9;
	MinAreaSize = 3;
	MinSpecialAreaSize = 4;
	UseDoublePath = false;
	UseBranchExit = false;
	UseLoopBranchPath = false;
	MultiLayerBranchPath = false;
	IsImpasse = false;
	SecondaryAreaRatio = 0.0f;

	m_strDungeonNodeName = "";
	m_nNodeDepth	= 0;
	m_bIsBossDungeonNode = false;

	m_pParentNode	= nullptr;		///¸¸½Úµã
	m_pLeftNode		= nullptr;		///main path
	m_pRightNode	= nullptr;		///branch path
}


// Called when the game starts
void UDungeonNodeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDungeonNodeComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

