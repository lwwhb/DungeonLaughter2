// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "DungeonRoot.h"
#include "../Private/DungeonGenerator.h"
#include "../Private/AlisaMethod.h"
#include "DL2_HUD.h"
#include "TerrainTile.h"
#include "DL2_PlayerStart.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"

// Sets default values
ADungeonRoot::ADungeonRoot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DungeonType = EDungeonType::DTE_Sample;
	MaxDungeonDepth = 1;
	CellUnit = 100;
	RandomGenerate = false;
	RandomMinDungeonDepth = 1;
	RandomMaxDungeonDepth = 5;
	RandomGenerateType = ERandomGenerateType::RGTE_CompletedRandom;
	RandomMinCellCountX = 32;
	RandomMaxCellCountX = 96;
	RandomMinCellCountY = 32;
	RandomMaxCellCountY = 96;
	RandomMinSplitAreaSize = 7;
	RandomMaxSplitAreaSize = 9;

	DungeonPatternArray.Add(FConversations());

	m_pDungeonRoot				= nullptr;
	m_pCurrentDungeonNode		= nullptr;

	m_strDungeonName			= "";
	m_strBossDungeonName		= "";
	m_strBranchDungeonName		= "";
	m_strBranchBossDungeonName	= "";

	m_nDungeonPatternIndex = (uint8)EDungeonPatternType::DPTE_Pattern0;
}
ADungeonRoot::~ADungeonRoot()
{
	DungeonPatternArray.Empty();
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

bool ADungeonRoot::generateDungeon()
{
	if (!m_pDungeonRoot)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Call initRootDungeonNode function first!"));
		return false;
	}
	if (!m_pCurrentDungeonNode)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Error! CurrentDungeonNode is null!"));
		return false;
	}
	if (!DungeonGenerator::getInstance()->setGeneratorSetting(
		DungeonType,
		m_pCurrentDungeonNode->CellCountX,
		m_pCurrentDungeonNode->CellCountY,
		CellUnit,
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
	{
		UE_LOG(LogTemp, Fatal, TEXT("Set dungeon setting failed!"));
		return false;
	}
	if (!DungeonGenerator::getInstance()->generateDungeon2dData(m_pCurrentDungeonNode->m_Map))
	{
		UE_LOG(LogTemp, Fatal, TEXT("Generate dungeon 2d data failed!"));
		return false;
	}
	m_pCurrentDungeonNode->copyAreaInfos();
	m_pCurrentDungeonNode->copyStatisticsData();
	if (!buildMap())
	{
		UE_LOG(LogTemp, Fatal, TEXT("Build dungeon map failed!"));
		return false;
	}
	return true;
}
bool ADungeonRoot::generateRandomDungeonTree()
{
	if (!RandomGenerate)
		return false;
	if (m_pDungeonRoot)
		return false;
	if (RandomMinDungeonDepth > RandomMaxDungeonDepth)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("RandomMinDungeonDepth value must be less than RandomMaxDungeonDepth value.")));
		return false;
	}
	if (RandomMinCellCountX > RandomMaxCellCountX)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("RandomMinCellCountX value must be less than RandomMaxCellCountX value.")));
		return false;
	}
	if (RandomMinCellCountY > RandomMaxCellCountY)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("RandomMinCellCountY value must be less than RandomMaxCellCountY value.")));
		return false;
	}
	if (RandomMinSplitAreaSize > RandomMaxSplitAreaSize)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("RandomMinSplitAreaSize value must be less than RandomMaxSplitAreaSize value.")));
		return false;
	}
	bool ret = false;
	int maxDepth = FMath::RandRange(RandomMinDungeonDepth, RandomMaxDungeonDepth);
	m_pDungeonRoot = NewObject<UDungeonNodeComponent>(this, TEXT("DungeonNodeLayer1Root"));
	if (m_pDungeonRoot)
	{
		AddComponent(TEXT("UDungeonNodeComponent"), false, FTransform::Identity, m_pDungeonRoot);
		if (maxDepth == 1)
		{
			std::shared_ptr<AlisaMethod> am = AlisaMethod::create(0.7, 0.3);
			int index = am.get()->getRandomIndex();
			m_pDungeonRoot->DungeonStyle = (EDungeonStyle)index;
			am = AlisaMethod::create(0.05, 0.95);
			index = am.get()->getRandomIndex();
			m_pDungeonRoot->IsBossDungeonNode = (index == 0);
			if(m_pDungeonRoot->IsBossDungeonNode)	//注意将来特殊boss层对是否死路单独处理
				m_pDungeonRoot->IsImpasse = FMath::RandBool();
		}
		else
		{
			std::shared_ptr<AlisaMethod> am = AlisaMethod::create(0.6, 0.3, 0.1);
			int index = am.get()->getRandomIndex();
			m_pDungeonRoot->DungeonStyle = (EDungeonStyle)index;
		}
		if (RandomGenerateType == ERandomGenerateType::RGTE_CompletedRandom)
		{
			m_pDungeonRoot->CellCountX = FMath::RandRange(RandomMinCellCountX, RandomMaxCellCountX);
			m_pDungeonRoot->CellCountY = FMath::RandRange(RandomMinCellCountY, RandomMaxCellCountY);
			m_pDungeonRoot->UseDoublePath = FMath::RandBool();
			m_pDungeonRoot->UseBranchExit = FMath::RandBool();
			if (m_pDungeonRoot->UseBranchExit)
			{
				m_pDungeonRoot->UseLoopBranchPath = FMath::RandBool();
				m_pDungeonRoot->MultiLayerBranchPath = FMath::RandBool();
			}
			m_pDungeonRoot->SecondaryAreaRatio = FMath::RandRange(0.0f, 0.5f);
		}
		else if (RandomGenerateType == ERandomGenerateType::RGTE_ComplexityAscending)
		{
			m_pDungeonRoot->CellCountX = RandomMinCellCountX;
			m_pDungeonRoot->CellCountY = RandomMinCellCountY;
			std::shared_ptr<AlisaMethod> am = AlisaMethod::create(0.1, 0.9);
			int index = am.get()->getRandomIndex();
			m_pDungeonRoot->UseDoublePath = (index == 0);
			am = AlisaMethod::create(0.1, 0.9);
			index = am.get()->getRandomIndex();
			m_pDungeonRoot->UseBranchExit = (index == 0);
			if (m_pDungeonRoot->UseBranchExit)
			{
				m_pDungeonRoot->UseLoopBranchPath = FMath::RandBool();
				m_pDungeonRoot->MultiLayerBranchPath = FMath::RandBool();
			}
			m_pDungeonRoot->SecondaryAreaRatio = FMath::RandRange(0.0f, 0.1f);
		}
		m_pDungeonRoot->MinSplitAreaSize = FMath::RandRange(RandomMinSplitAreaSize, RandomMaxSplitAreaSize);
		m_pDungeonRoot->MaxSplitAreaSize = FMath::RandRange(FMath::Max(m_pDungeonRoot->MinSplitAreaSize, 8), RandomMaxSplitAreaSize);
		m_pDungeonRoot->MinAreaSize = FMath::Max(m_pDungeonRoot->MinAreaSize, FMath::RandRange(m_pDungeonRoot->MinAreaSize, (m_pDungeonRoot->MinSplitAreaSize - 1)/2));
		m_pDungeonRoot->MinSpecialAreaSize = FMath::Min(m_pDungeonRoot->MinAreaSize + 1, m_pDungeonRoot->MinSplitAreaSize);

		m_pDungeonRoot->NodeDepth = 1;
		MaxDungeonDepth = FMath::Max(MaxDungeonDepth, m_pDungeonRoot->NodeDepth);

		if (m_pDungeonRoot->NodeDepth < maxDepth)
		{
			ret = generateLeftRandomDungeonNode(m_pDungeonRoot, maxDepth);
			if (m_pDungeonRoot->UseBranchExit && m_pDungeonRoot->MultiLayerBranchPath)
				ret = generateRightRandomDungeonNode(m_pDungeonRoot, maxDepth);
		}
		else
			ret = true;
	}
	return ret;
}
bool ADungeonRoot::generateLeftRandomDungeonNode(UDungeonNodeComponent* parrent, int maxDepth)
{
	if (!parrent)
		return false;
	if (parrent->m_pLeftNode)
		return false;
	bool ret = false;
	int nodeDepth = parrent->NodeDepth + 1;
	FString name = "DungeonNodeLayer" + FString::FromInt(nodeDepth);
	name = name + "Left";
	parrent->m_pLeftNode = NewObject<UDungeonNodeComponent>(this, FName(*name));
	if (parrent->m_pLeftNode)
	{
		parrent->m_pLeftNode->m_pParentNode = parrent;
		AddComponent(TEXT("UDungeonNodeComponent"), false, FTransform::Identity, parrent->m_pLeftNode);
		std::shared_ptr<AlisaMethod> am = (nodeDepth == maxDepth) ? AlisaMethod::create(0.7, 0.3) : AlisaMethod::create(0.6, 0.3, 0.1);
		int index = am.get()->getRandomIndex();
		parrent->m_pLeftNode->DungeonStyle = (EDungeonStyle)index;
		am = AlisaMethod::create(0.05*nodeDepth, 1.0 - 0.05*nodeDepth);
		index = am.get()->getRandomIndex();
		parrent->m_pLeftNode->IsBossDungeonNode = (index == 0);
		if (parrent->m_pLeftNode->IsBossDungeonNode)	//注意将来特殊boss层对是否死路单独处理
			parrent->m_pLeftNode->IsImpasse = FMath::RandBool();
	}
	if (RandomGenerateType == ERandomGenerateType::RGTE_CompletedRandom)
	{
		parrent->m_pLeftNode->CellCountX = FMath::RandRange(RandomMinCellCountX, RandomMaxCellCountX);
		parrent->m_pLeftNode->CellCountY = FMath::RandRange(RandomMinCellCountY, RandomMaxCellCountY);
		parrent->m_pLeftNode->UseDoublePath = FMath::RandBool();
		parrent->m_pLeftNode->UseBranchExit = FMath::RandBool();
		if (parrent->m_pLeftNode->UseBranchExit)
		{
			parrent->m_pLeftNode->UseLoopBranchPath = FMath::RandBool();
			parrent->m_pLeftNode->MultiLayerBranchPath = FMath::RandBool();
		}
		parrent->m_pLeftNode->SecondaryAreaRatio = FMath::RandRange(0.0f, 0.5f);
	}
	else if (RandomGenerateType == ERandomGenerateType::RGTE_ComplexityAscending)
	{
		parrent->m_pLeftNode->CellCountX = FMath::Min(RandomMinCellCountX + (int)((RandomMaxCellCountX - RandomMinCellCountX)*((float)(nodeDepth - 1) / (float)maxDepth)), RandomMaxCellCountX);
		parrent->m_pLeftNode->CellCountY = FMath::Min(RandomMinCellCountY + (int)((RandomMaxCellCountY - RandomMinCellCountY)*((float)(nodeDepth - 1) / (float)maxDepth)), RandomMaxCellCountY);
		std::shared_ptr<AlisaMethod> am = AlisaMethod::create(0.1*nodeDepth, 1.0 - 0.1*nodeDepth);
		int index = am.get()->getRandomIndex();
		parrent->m_pLeftNode->UseDoublePath = (index == 0);
		am = AlisaMethod::create(0.1*nodeDepth, 1.0 - 0.1*nodeDepth);
		index = am.get()->getRandomIndex();
		parrent->m_pLeftNode->UseBranchExit = (index == 0);
		if (parrent->m_pLeftNode->UseBranchExit)
		{
			parrent->m_pLeftNode->UseLoopBranchPath = FMath::RandBool();
			parrent->m_pLeftNode->MultiLayerBranchPath = FMath::RandBool();
		}
		parrent->m_pLeftNode->SecondaryAreaRatio = FMath::RandRange(FMath::Max(0.1f*parrent->NodeDepth, 0.0f), FMath::Min(0.1f*nodeDepth, 0.5f));
	}
	parrent->m_pLeftNode->MinSplitAreaSize = FMath::RandRange(RandomMinSplitAreaSize, RandomMaxSplitAreaSize);
	parrent->m_pLeftNode->MaxSplitAreaSize = FMath::RandRange(FMath::Max(parrent->m_pLeftNode->MinSplitAreaSize, 8), RandomMaxSplitAreaSize);
	parrent->m_pLeftNode->MinAreaSize = FMath::Max(parrent->m_pLeftNode->MinAreaSize, FMath::RandRange(parrent->m_pLeftNode->MinAreaSize, (parrent->m_pLeftNode->MinSplitAreaSize - 1) / 2));
	parrent->m_pLeftNode->MinSpecialAreaSize = FMath::Min(parrent->m_pLeftNode->MinAreaSize + 1, parrent->m_pLeftNode->MinSplitAreaSize);
	parrent->m_pLeftNode->NodeDepth = nodeDepth;
	MaxDungeonDepth = FMath::Max(MaxDungeonDepth, parrent->m_pLeftNode->NodeDepth);

	if (parrent->m_pLeftNode->NodeDepth < maxDepth)
	{
		if (!parrent->m_pLeftNode->IsImpasse)
		{
			ret = generateLeftRandomDungeonNode(parrent->m_pLeftNode, maxDepth);
			if (parrent->m_pLeftNode->UseBranchExit && parrent->m_pLeftNode->MultiLayerBranchPath)
				ret = generateRightRandomDungeonNode(parrent->m_pLeftNode, maxDepth);
		}
		else
			ret = true;
	}
	else
		ret = true;
	return ret;
}
bool ADungeonRoot::generateRightRandomDungeonNode(UDungeonNodeComponent* parrent, int maxDepth)
{
	if (!parrent)
		return false;
	if (parrent->m_pRightNode)
		return false;
	bool ret = false;
	int nodeDepth = parrent->NodeDepth + 1;
	FString name = "DungeonNodeLayer" + FString::FromInt(nodeDepth);
	name = name + "Right";
	parrent->m_pRightNode = NewObject<UDungeonNodeComponent>(this, FName(*name));
	if (parrent->m_pRightNode)
	{
		parrent->m_pRightNode->m_pParentNode = parrent;
		AddComponent(TEXT("UDungeonNodeComponent"), false, FTransform::Identity, parrent->m_pRightNode);
		std::shared_ptr<AlisaMethod> am = (nodeDepth == maxDepth) ? AlisaMethod::create(0.7, 0.3) : AlisaMethod::create(0.6, 0.3, 0.1);
		int index = am.get()->getRandomIndex();
		parrent->m_pRightNode->DungeonStyle = (EDungeonStyle)index;
	}
	if (RandomGenerateType == ERandomGenerateType::RGTE_CompletedRandom)
	{
		parrent->m_pRightNode->CellCountX = FMath::RandRange(RandomMinCellCountX, RandomMaxCellCountX);
		parrent->m_pRightNode->CellCountY = FMath::RandRange(RandomMinCellCountY, RandomMaxCellCountY);
		parrent->m_pRightNode->UseDoublePath = FMath::RandBool();
		parrent->m_pRightNode->UseBranchExit = FMath::RandBool();
		if (parrent->m_pRightNode->UseBranchExit)
		{
			parrent->m_pRightNode->UseLoopBranchPath = FMath::RandBool();
			parrent->m_pRightNode->MultiLayerBranchPath = FMath::RandBool();
		}
		parrent->m_pRightNode->SecondaryAreaRatio = FMath::RandRange(0.0f, 0.5f);
	}
	else if (RandomGenerateType == ERandomGenerateType::RGTE_ComplexityAscending)
	{
		parrent->m_pRightNode->CellCountX = FMath::Min(RandomMinCellCountX + (int)((RandomMaxCellCountX - RandomMinCellCountX)*((float)(nodeDepth-1)/(float)maxDepth)), RandomMaxCellCountX);
		parrent->m_pRightNode->CellCountY = FMath::Min(RandomMinCellCountY + (int)((RandomMaxCellCountY - RandomMinCellCountY)*((float)(nodeDepth - 1) / (float)maxDepth)), RandomMaxCellCountY);
		std::shared_ptr<AlisaMethod> am = AlisaMethod::create(0.1*nodeDepth, 1.0 - 0.1*nodeDepth);
		int index = am.get()->getRandomIndex();
		parrent->m_pRightNode->UseDoublePath = (index == 0);
		am = AlisaMethod::create(0.1*nodeDepth, 1.0 - 0.1*nodeDepth);
		index = am.get()->getRandomIndex();
		parrent->m_pRightNode->UseBranchExit = (index == 0);
		if (parrent->m_pRightNode->UseBranchExit)
		{
			parrent->m_pRightNode->UseLoopBranchPath = FMath::RandBool();
			parrent->m_pRightNode->MultiLayerBranchPath = FMath::RandBool();
		}
		parrent->m_pRightNode->SecondaryAreaRatio = FMath::RandRange(FMath::Max(0.1f*parrent->NodeDepth, 0.0f), FMath::Min(0.1f*nodeDepth, 0.5f));
	}
	parrent->m_pRightNode->MinSplitAreaSize = FMath::RandRange(RandomMinSplitAreaSize, RandomMaxSplitAreaSize);
	parrent->m_pRightNode->MaxSplitAreaSize = FMath::RandRange(FMath::Max(parrent->m_pRightNode->MinSplitAreaSize, 8), RandomMaxSplitAreaSize);
	parrent->m_pRightNode->MinAreaSize = FMath::Max(parrent->m_pRightNode->MinAreaSize, FMath::RandRange(parrent->m_pRightNode->MinAreaSize, (parrent->m_pRightNode->MinSplitAreaSize - 1) / 2));
	parrent->m_pRightNode->MinSpecialAreaSize = FMath::Min(parrent->m_pRightNode->MinAreaSize + 1, parrent->m_pRightNode->MinSplitAreaSize);
	parrent->m_pRightNode->NodeDepth = nodeDepth;
	MaxDungeonDepth = FMath::Max(MaxDungeonDepth, parrent->m_pRightNode->NodeDepth);

	if (parrent->m_pRightNode->NodeDepth < maxDepth)
	{
		if (parrent->m_pRightNode->UseBranchExit && parrent->m_pRightNode->MultiLayerBranchPath)
			ret = generateRightRandomDungeonNode(parrent->m_pRightNode, maxDepth);
		else
			ret = true;
	}
	else
		ret = true;
	return ret;
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
	if (dungeonNode->IsImpasse)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("If Parent Node is impasse node, Can't add left dungeon node.")));
		return false;
	}
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
	if (!dungeonNode->MultiLayerBranchPath)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("If Parent Node isn't multiLayer branch path, Can't add right dungeon node.")));
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
	{
		if (!generateDungeon())
			return false;
#if WITH_EDITOR
		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		if (playerController)
		{
			ADL2_HUD* hud = static_cast<ADL2_HUD*>(playerController->GetHUD());
			if (hud)
				hud->EnableSteps();
		}
#endif // WITH_EDITOR
	}
	else
	{
		if (!buildMap())
		{
			UE_LOG(LogTemp, Fatal, TEXT("Build dungeon map failed!"));
			return false;
		}
#if WITH_EDITOR
		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		if (playerController)
		{
			ADL2_HUD* hud = static_cast<ADL2_HUD*>(playerController->GetHUD());
			if (hud)
				hud->DisableSteps();
		}
#endif // WITH_EDITOR
	}
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
		{
			if (!generateDungeon())
				return false;
#if WITH_EDITOR
			APlayerController* playerController = GetWorld()->GetFirstPlayerController();
			if (playerController)
			{
				ADL2_HUD* hud = static_cast<ADL2_HUD*>(playerController->GetHUD());
				if (hud)
					hud->EnableSteps();
			}
#endif // WITH_EDITOR
		}
		else
		{
			if (!buildMap())
			{
				UE_LOG(LogTemp, Fatal, TEXT("Build dungeon map failed!"));
				return false;
			}
#if WITH_EDITOR
			APlayerController* playerController = GetWorld()->GetFirstPlayerController();
			if (playerController)
			{
				ADL2_HUD* hud = static_cast<ADL2_HUD*>(playerController->GetHUD());
				if (hud)
					hud->DisableSteps();
			}
#endif // WITH_EDITOR
		}
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
		{
			if (!generateDungeon())
				return false;
#if WITH_EDITOR
			APlayerController* playerController = GetWorld()->GetFirstPlayerController();
			if (playerController)
			{
				ADL2_HUD* hud = static_cast<ADL2_HUD*>(playerController->GetHUD());
				if (hud)
					hud->EnableSteps();
			}
#endif // WITH_EDITOR
		}
		else
		{
			if (!buildMap())
			{
				UE_LOG(LogTemp, Fatal, TEXT("Build dungeon map failed!"));
				return false;
			}
#if WITH_EDITOR
			APlayerController* playerController = GetWorld()->GetFirstPlayerController();
			if (playerController)
			{
				ADL2_HUD* hud = static_cast<ADL2_HUD*>(playerController->GetHUD());
				if (hud)
					hud->DisableSteps();
			}
#endif // WITH_EDITOR
		}
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
	if (m_pCurrentDungeonNode->m_Map.empty())
	{
		if (!generateDungeon())
			return false;
#if WITH_EDITOR
		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		if (playerController)
		{
			ADL2_HUD* hud = static_cast<ADL2_HUD*>(playerController->GetHUD());
			if (hud)
				hud->EnableSteps();
		}
#endif // WITH_EDITOR
	}
	else
	{
		if (!buildMap())
		{
			UE_LOG(LogTemp, Fatal, TEXT("Build dungeon map failed!"));
			return false;
		}
#if WITH_EDITOR
		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		if (playerController)
		{
			ADL2_HUD* hud = static_cast<ADL2_HUD*>(playerController->GetHUD());
			if (hud)
				hud->DisableSteps();
		}
#endif // WITH_EDITOR
	}
	return true;
}

bool ADungeonRoot::buildMap()
{
	if (!m_pCurrentDungeonNode)
		return false;

	if (!clearMap())
	{
		UE_LOG(LogTemp, Fatal, TEXT("Clear map failed!"));
		return false;
	}
	calculateDungeonPatternIndex();
	for (int i = 0; i < m_pCurrentDungeonNode->m_Map.size(); ++i)
	{
		if (!buildTerrainTile(m_pCurrentDungeonNode->m_Map[i]))
		{
			UE_LOG(LogTemp, Fatal, TEXT("Build terrain tile %d failed!"), i);
			return false;
		}
		if (!buildEntrance(m_pCurrentDungeonNode->m_Map[i]))
		{
			UE_LOG(LogTemp, Fatal, TEXT("Build entrance terrain tile %d failed!"), i);
			return false;
		}
	}
	if (!rebuildNavigationMesh())
	{
		UE_LOG(LogTemp, Fatal, TEXT("Rebuild navigation mesh failed!"));
		return false;
	}
	return true;
}
bool ADungeonRoot::clearMap()
{
	for (int i = 0; i < m_TerrainTileArray.Num(); ++i)
	{
		if (!(GetWorld()->DestroyActor(m_TerrainTileArray[i])))
			return false;
	}
	while (m_TerrainTileArray.Num() > 0)
		m_TerrainTileArray.RemoveAt(0);
	return true;
}
bool ADungeonRoot::rebuildNavigationMesh()
{
	if (!m_pCurrentDungeonNode)
		return false;
	ULevel* curLevel = GetWorld()->GetCurrentLevel();
	if (!curLevel)
		return false;
	ANavMeshBoundsVolume* volume = nullptr;
	for (AActor* actor : curLevel->Actors)
	{
		if (actor && actor->IsA(ANavMeshBoundsVolume::StaticClass()))
		{
			volume = Cast<ANavMeshBoundsVolume>(actor);
			break;
		}
	}
	if (!volume)
		return false;
	volume->GetRootComponent()->SetMobility(EComponentMobility::Movable);
	int width = m_pCurrentDungeonNode->CellCountX * CellUnit;
	int height = m_pCurrentDungeonNode->CellCountY * CellUnit;
	FVector min = FVector(-height*0.5f, -width*0.5f, -CellUnit * 5);
	FVector max = FVector(height*0.5f, width*0.5f, CellUnit * 5);
	FVector pos = FVector(-height*0.5f, width*0.5f, 0);
	volume->GetBrushComponent()->Bounds = FBox(min, max).TransformBy(FTransform(pos));
	GetWorld()->GetNavigationSystem()->OnNavigationBoundsUpdated(volume);
	volume->GetRootComponent()->SetMobility(EComponentMobility::Static);

	for (int i = 0; i < m_TerrainTileArray.Num(); ++i)
	{
		ATerrainTile* terrainTile = m_TerrainTileArray[i];
		if (!terrainTile)
			return false;
		if (terrainTile->CellType == ECellTypeEnum::CTE_StandardWall || terrainTile->CellType == ECellTypeEnum::CTE_PassageWall)
			continue;
		terrainTile->GetRootComponent()->SetMobility(EComponentMobility::Movable);
		terrainTile->SetActorLocation(terrainTile->GetActorLocation() + FVector(0, 0, -CellUnit));
		terrainTile->GetRootComponent()->SetMobility(EComponentMobility::Stationary);
	}
	return true;
}
void ADungeonRoot::calculateDungeonPatternIndex()
{
	if (!m_pCurrentDungeonNode)
		return;
	if(DungeonPatternArray.Num() < 1)
		UE_LOG(LogTemp, Fatal, TEXT("DungeonPatternArray size must be more than 1"));

	UE_LOG(LogTemp, Display, TEXT("DungeonPatternIndex = %d"), m_nDungeonPatternIndex);
}
ATerrainTile* ADungeonRoot::findTerrianTileByCellType(ECellTypeEnum cellType)
{
	for (int i = 0; i < m_TerrainTileArray.Num(); ++i)
	{
		if (m_TerrainTileArray[i]->CellType == cellType)
			return m_TerrainTileArray[i];
	}
	return nullptr;
}
bool ADungeonRoot::buildTerrainTile(const Cell& cell)
{
	UClass* classType = nullptr;
	ATerrainTile* terrainTile = nullptr;
	FActorSpawnParameters spawnParms;
	spawnParms.Owner = this;
	switch (cell.getCellType())
	{
	case ECellTypeEnum::CTE_EmptyCell:
		break;
	case ECellTypeEnum::CTE_StandardFloor:
	case ECellTypeEnum::CTE_PassageFloor:
	case ECellTypeEnum::CTE_TunnelFloor:
	case ECellTypeEnum::CTE_StandardDoor:
	case ECellTypeEnum::CTE_Entrance:
	case ECellTypeEnum::CTE_Exit:
	case ECellTypeEnum::CTE_BranchExit:
	case ECellTypeEnum::CTE_LockedDoor:
	case ECellTypeEnum::CTE_MissionLockedDoor:
	case ECellTypeEnum::CTE_Barricade:
	case ECellTypeEnum::CTE_BossTreasuryDoor:
		terrainTile = findTerrianTileByCellType(ECellTypeEnum::CTE_StandardFloor);
		if (!terrainTile)
		{
			classType = DungeonPatternArray[m_nDungeonPatternIndex].StandardFloorBlueprintsArray[0];
			terrainTile = GetWorld()->SpawnActor<ATerrainTile>(classType, spawnParms);
			if (!terrainTile)
				return false;
			terrainTile->CellType = cell.getCellType();
			terrainTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			m_TerrainTileArray.Add(terrainTile);
		}
		break;
	case ECellTypeEnum::CTE_Water:
	case ECellTypeEnum::CTE_DeepWater:
		break;
	case ECellTypeEnum::CTE_StandardWall:
	case ECellTypeEnum::CTE_PassageWall:
	case ECellTypeEnum::CTE_HiddenDoor:
		terrainTile = findTerrianTileByCellType(ECellTypeEnum::CTE_StandardWall);
		if (!terrainTile)
		{
			classType = DungeonPatternArray[m_nDungeonPatternIndex].StandardWallBlueprintsArray[0];
			terrainTile = GetWorld()->SpawnActor<ATerrainTile>(classType, spawnParms);
			if (!terrainTile)
				return false;
			terrainTile->CellType = cell.getCellType();
			terrainTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			m_TerrainTileArray.Add(terrainTile);
		}
		break;
	default:
		break;
	}

	if (terrainTile)
	{
		FTransform transform;
		transform.SetLocation((FVector(-cell.getIndexY()*CellUnit, cell.getIndexX()*CellUnit, 0)));
		transform.SetScale3D(terrainTile->GetActorScale()*((float)CellUnit / 100.0f));
		if (!terrainTile->addInstance(transform))
			return false;
	}
	return true;
}
bool ADungeonRoot::buildEntrance(const Cell& cell)
{
	if (cell.getCellType() == ECellTypeEnum::CTE_Entrance)
	{
		APlayerController* localPlayerController = GetWorld()->GetFirstPlayerController();
		if (localPlayerController)
			localPlayerController->GetPawn()->SetActorLocation((FVector(-cell.getIndexY()*CellUnit, cell.getIndexX()*CellUnit, -CellUnit*0.5f )));
	}
	return true;
}