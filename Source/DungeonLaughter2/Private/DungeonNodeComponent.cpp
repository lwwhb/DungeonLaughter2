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
	IsBossDungeonNode = false;

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
	Regenerate = false;

	DungeonNodeName = "";
	NodeDepth	= 0;

	m_pParentNode	= nullptr;		///¸¸½Úµã
	m_pLeftNode		= nullptr;		///main path
	m_pRightNode	= nullptr;		///branch path
	m_VisitedType	= EVisitedType::VTE_NO;

	m_nEntranceAreaCount	= 0;
	m_nExitAreaCount		= 0;
	m_nBranchAreaCount		= 0;
	m_nMainPathAreaCount	= 0;
	m_nSidePathAreaCount	= 0;
	m_nBranchPathAreaCount	= 0;
	m_nSecondaryAreaCount	= 0;
	m_nPivotalAreaCount		= 0;
	m_nSpecialAreaCount		= 0;
	m_nUnusualAreaCount		= 0;
	m_nStandardAreaCount	= 0;
	m_nPassageAreaCount		= 0;
	m_nTunnelAreaCount		= 0;
	m_nStandardDoorCount	= 0;
	m_nLockedDoorCount		= 0;
	m_nHiddenDoorCount		= 0;
	m_nSpecialDoorCount		= 0;
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
void UDungeonNodeComponent::copyAreaInfos()
{
	m_AreaInfos.clear();
	std::vector<Area*> connectedAreas = DungeonGenerator::getInstance()->getConnectedAreas();
	for (size_t i = 0; i < connectedAreas.size(); i++)
	{
		Area* area = connectedAreas[i];
		if (area)
		{
			AreaInfo areaInfo;
			areaInfo.setRect(area->getRect());
			areaInfo.setAreaType(area->getAreaType());
			areaInfo.setAreaTypeMask(area->getAreaTypeMask());
			std::unordered_map<Area*, Door*>  connectedNeigbourAreas = area->getConnectedAreas();
			for (auto iter = connectedNeigbourAreas.begin(); iter != connectedNeigbourAreas.end(); iter++) {
				Area* neigbourArea = iter->first;
				Door* door = iter->second;
				if (neigbourArea && door)
					areaInfo.m_DoorInfos.push_back(*door);
			}
			m_AreaInfos.push_back(areaInfo);
		}
	}
}
void UDungeonNodeComponent::copyStatisticsData()
{
	Area* entranceArea = DungeonGenerator::getInstance()->getEntranceArea();
	if (entranceArea)
		m_nEntranceAreaCount = 1;
	else
		m_nEntranceAreaCount = 0;

	Area* exitArea = DungeonGenerator::getInstance()->getExitArea();
	if (exitArea)
		m_nExitAreaCount = 1;
	else
		m_nExitAreaCount = 0;

	Area* branchArea = DungeonGenerator::getInstance()->getBranchArea();
	if (branchArea)
		m_nBranchAreaCount = 1;
	else
		m_nBranchAreaCount = 0;

	m_nMainPathAreaCount = DungeonGenerator::getInstance()->getMainPathAreaCount();
	m_nSidePathAreaCount = DungeonGenerator::getInstance()->getSidePathAreaCount();
	m_nBranchPathAreaCount = DungeonGenerator::getInstance()->getBranchPathAreaCount();
	m_nSecondaryAreaCount = DungeonGenerator::getInstance()->getSecondaryAreaCount();
	m_nPivotalAreaCount = DungeonGenerator::getInstance()->getPivotalAreaCount();
	m_nSpecialAreaCount = DungeonGenerator::getInstance()->getSpecialAreaCount();
	m_nUnusualAreaCount = DungeonGenerator::getInstance()->getUnusualAreaCount();
	m_nStandardAreaCount = DungeonGenerator::getInstance()->getStandardAreaCount();
	m_nPassageAreaCount = DungeonGenerator::getInstance()->getPassageAreaCount();
	m_nTunnelAreaCount = DungeonGenerator::getInstance()->getTunnelAreaCount();
	m_nStandardDoorCount = DungeonGenerator::getInstance()->getStandardDoorCount();
	m_nLockedDoorCount = DungeonGenerator::getInstance()->getLockedDoorCount();
	m_nHiddenDoorCount = DungeonGenerator::getInstance()->getHiddenDoorCount();
	m_nSpecialDoorCount = DungeonGenerator::getInstance()->getSpecialDoorCount();
}