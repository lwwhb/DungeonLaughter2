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
	RandomGenerate = false;
	RandomGenerateType = ERandomGenerateType::RGTE_CompletedRandom;

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
	if(!DungeonGenerator::getInstance()->generateDungeon(m_pCurrentDungeonNode->m_Map))
		UE_LOG(LogTemp, Fatal, TEXT("Generate dungeon failed!"));
}
bool ADungeonRoot::generateRandomDungeonTree(UDungeonNodeComponent* dungeonNode)
{
	if (!initRootDungeonNode(dungeonNode))
		return false;
	return true;
}
bool ADungeonRoot::initRootDungeonNode(UDungeonNodeComponent* dungeonNode)
{
	if (!dungeonNode)
		return false;
	m_pDungeonRoot = dungeonNode;
	m_pDungeonRoot->NodeDepth = 1;
	MaxDungeonDepth = FMath::Max(MaxDungeonDepth, m_pDungeonRoot->NodeDepth);
	return true;
}
bool ADungeonRoot::addLeftDungeonNode(UDungeonNodeComponent* dungeonNode, UDungeonNodeComponent* leftDungeonNode)
{
	if (!dungeonNode || !m_pDungeonRoot || !leftDungeonNode)
		return false;
	dungeonNode->m_pLeftNode = leftDungeonNode;
	dungeonNode->m_pLeftNode->m_pParentNode = dungeonNode;
	dungeonNode->m_pLeftNode->NodeDepth = dungeonNode->NodeDepth + 1;
	MaxDungeonDepth = FMath::Max(MaxDungeonDepth, dungeonNode->m_pLeftNode->NodeDepth);
	return true;
}

bool ADungeonRoot::addRightDungeonNode(UDungeonNodeComponent* dungeonNode, UDungeonNodeComponent* rightDungeonNode)
{
	if (!dungeonNode || !m_pDungeonRoot || !rightDungeonNode)
		return false;
	if (!dungeonNode->UseBranchExit)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("If Parent Node doesn't use branch exit, Can't add right dungeon node.")));
		return false;
	}
	dungeonNode->m_pRightNode = rightDungeonNode;
	dungeonNode->m_pRightNode->m_pParentNode = dungeonNode;
	dungeonNode->m_pRightNode->NodeDepth = dungeonNode->NodeDepth + 1;
	MaxDungeonDepth = FMath::Max(MaxDungeonDepth, dungeonNode->m_pRightNode->NodeDepth);
	return true;
}
bool ADungeonRoot::enterDungeon()
{
	if (!m_pDungeonRoot || m_pCurrentDungeonNode != nullptr)
		return false;
	m_pCurrentDungeonNode = m_pDungeonRoot;
	m_pCurrentDungeonNode->m_VisitedType = EVisitedType::VTE_YES;

	if (m_pCurrentDungeonNode->Regenerate)
		m_pCurrentDungeonNode->m_Map.clear();
	if (m_pCurrentDungeonNode->m_Map.empty())
		GenerateDungeon2dData();
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
		m_pCurrentDungeonNode->m_VisitedType = EVisitedType::VTE_YES;
		depth = m_pCurrentDungeonNode->NodeDepth;

		if (m_pCurrentDungeonNode->Regenerate)
			m_pCurrentDungeonNode->m_Map.clear();
		if (m_pCurrentDungeonNode->m_Map.empty())
			GenerateDungeon2dData();
	}
	else
	{
		if (!m_pCurrentDungeonNode->m_pLeftNode)
			return false;
		m_pCurrentDungeonNode = m_pCurrentDungeonNode->m_pLeftNode;
		m_pCurrentDungeonNode->m_VisitedType = EVisitedType::VTE_YES;
		depth = m_pCurrentDungeonNode->NodeDepth;

		if (m_pCurrentDungeonNode->Regenerate)
			m_pCurrentDungeonNode->m_Map.clear();
		if (m_pCurrentDungeonNode->m_Map.empty())
			GenerateDungeon2dData();
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
	depth = m_pCurrentDungeonNode->NodeDepth;

	if (m_pCurrentDungeonNode->Regenerate)
		m_pCurrentDungeonNode->m_Map.clear();
	if(m_pCurrentDungeonNode->m_Map.empty())
		GenerateDungeon2dData();
	return true;
}