// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "DungeonRoot.h"
#include "../Private/DungeonGenerator.h"

// Sets default values
ADungeonRoot::ADungeonRoot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DungeonType = EDungeonType::DTE_Test;
	MaxDungeonDepth = 1;
	GenerateBoss = false;

	m_pDungeonRoot				= nullptr;
	m_pCurrentDungeonNode		= nullptr;

	m_strDungeonName			= "";
	m_strBossDungeonName		= "";
	m_strBranchDungeonName		= "";
	m_strBranchBossDungeonName	= "";
}
ADungeonRoot::~ADungeonRoot()
{
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
	if(!m_pDungeonRoot)
		UE_LOG(LogTemp, Fatal, TEXT("Call initRootDungeonNode function first!"));
	if(!m_pCurrentDungeonNode)
		UE_LOG(LogTemp, Fatal, TEXT("Error! CurrentDungeonNode is null!"));
	if (!DungeonGenerator::getInstance()->setGeneratorSetting(
		m_pCurrentDungeonNode->CellCountX,
		m_pCurrentDungeonNode->CellCountY,
		m_pCurrentDungeonNode->CellUnit,
		m_pCurrentDungeonNode->MinSplitAreaSize,
		m_pCurrentDungeonNode->MaxSplitAreaSize,
		m_pCurrentDungeonNode->MinAreaSize,
		m_pCurrentDungeonNode->MinSpecialAreaSize,
		m_pCurrentDungeonNode->UseDoublePath,
		m_pCurrentDungeonNode->UseBranchExit,
		m_pCurrentDungeonNode->UseLoopBranchPath,
		m_pCurrentDungeonNode->MultiLayerBranchPath,
		m_pCurrentDungeonNode->IsImpasse,
		m_pCurrentDungeonNode->SecondaryAreaRatio,
		m_pCurrentDungeonNode->DungeonStyle
		))
		return;
	if(!DungeonGenerator::getInstance()->generateDungeon())
		UE_LOG(LogTemp, Fatal, TEXT("Generate dungeon failed!"));
}

bool ADungeonRoot::initRootDungeonNode(UDungeonNodeComponent* dungeonNode)
{
	if (!dungeonNode)
		return false;
	m_pDungeonRoot = dungeonNode;
	m_pCurrentDungeonNode = m_pDungeonRoot;
	m_pDungeonRoot->m_nNodeDepth = 1;
	return true;
}
bool ADungeonRoot::addLeftDungeonNode(UDungeonNodeComponent* dungeonNode, UDungeonNodeComponent* leftDungeonNode)
{
	if (!dungeonNode || !m_pDungeonRoot || !leftDungeonNode)
		return false;
	dungeonNode->m_pLeftNode = leftDungeonNode;
	dungeonNode->m_pLeftNode->m_pParentNode = dungeonNode;
	dungeonNode->m_pLeftNode->m_nNodeDepth = dungeonNode->m_nNodeDepth + 1;
	return true;
}

bool ADungeonRoot::addRightDungeonNode(UDungeonNodeComponent* dungeonNode, UDungeonNodeComponent* rightDungeonNode)
{
	if (!dungeonNode || !m_pDungeonRoot || !rightDungeonNode)
		return false;
	dungeonNode->m_pRightNode = rightDungeonNode;
	dungeonNode->m_pRightNode->m_pParentNode = dungeonNode;
	dungeonNode->m_pRightNode->m_nNodeDepth = dungeonNode->m_nNodeDepth + 1;
	return true;
}
bool ADungeonRoot::doDownstair(int& depth, bool goBranch)
{
	if (!m_pDungeonRoot || !m_pCurrentDungeonNode)
		return false;
	if (goBranch)
	{
		if (!m_pCurrentDungeonNode->m_pRightNode)
			return false;
		m_pCurrentDungeonNode = m_pCurrentDungeonNode->m_pRightNode;
		depth = m_pCurrentDungeonNode->m_nNodeDepth;
	}
	else
	{
		if (!m_pCurrentDungeonNode->m_pLeftNode)
			return false;
		m_pCurrentDungeonNode = m_pCurrentDungeonNode->m_pLeftNode;
		depth = m_pCurrentDungeonNode->m_nNodeDepth;
	}
	return true;
}

bool ADungeonRoot::doUpstair(int& depth)
{
	if (!m_pDungeonRoot || !m_pCurrentDungeonNode)
		return false;
	if (!m_pCurrentDungeonNode->m_pParentNode)
		return false;
	m_pCurrentDungeonNode = m_pCurrentDungeonNode->m_pParentNode;
	depth = m_pCurrentDungeonNode->m_nNodeDepth;
	return true;
}