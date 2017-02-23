// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "DL2_HUD.h"
#include "DungeonRoot.h"
#include "../Private/DungeonGenerator.h"
#include "../Private/Area.h"
#include "DungeonNodeComponent.h"

ADL2_HUD::ADL2_HUD()
{
	m_pDungeonRoot = nullptr;
}
void ADL2_HUD::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	EnableSteps();
	unit = DungeonGenerator::getInstance()->getDisplayCellUnit();
	UWorld* world = GetWorld();
	if (world)
	{
		int index = -1;
		if (!world->GetCurrentLevel()->Actors.FindItemByClass<ADungeonRoot>(&m_pDungeonRoot, &index))
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("This level need a ADungeonRoot object.")));
	}
#endif // WITH_EDITOR
}
void ADL2_HUD::DrawHUD()
{
	Super::DrawHUD();
#if WITH_EDITOR
	if (!m_pDungeonRoot || !m_pDungeonRoot->getCurrentDungeonNode())
		return;

	unit = DungeonGenerator::getInstance()->getDisplayCellUnit();
	if (showSteps)
	{
		if (enableStep1 && !enableStep7)
		{
			if (enableStep4)
				DrawConnectAreas();
			else
				DrawBackground();
		}
		if (enableStep3 && !enableStep7)
			DrawEntranceAreaAndExitArea();
		if (enableStep2 && !enableStep7)
			DrawSplitLines();
		if (enableStep7)
			DrawFinalMap();
		if (enableStep1)
		{
			DrawStatistics();
			DrawDungeonTreeInfo();
		}
	}
	else
	{
		DrawFinalMap();
		DrawStatistics();
		DrawDungeonTreeInfo();
	}
#endif // WITH_EDITOR
}

#if WITH_EDITOR

void ADL2_HUD::EnableSteps()
{
	showSteps = true;
	enableStep1 = false;
	enableStep2 = false;
	enableStep3 = false;
	enableStep4 = false;
	enableStep5 = false;
	enableStep6 = false;
	enableStep7 = false;
	GetWorldTimerManager().SetTimer(m_TimerHandle, this, &ADL2_HUD::EnableStep1, 1.0f, false);
}
void ADL2_HUD::DisableSteps()
{
	showSteps = false;
}
void ADL2_HUD::EnableStep1() 
{ 
	enableStep1 = true; 
	GetWorldTimerManager().SetTimer(m_TimerHandle, this, &ADL2_HUD::EnableStep2, 1.0f, false);
}
void ADL2_HUD::EnableStep2() 
{ 
	enableStep2 = true;
	GetWorldTimerManager().SetTimer(m_TimerHandle, this, &ADL2_HUD::EnableStep3, 1.0f, false);
}
void ADL2_HUD::EnableStep3() 
{ 
	enableStep3 = true; 
	GetWorldTimerManager().SetTimer(m_TimerHandle, this, &ADL2_HUD::EnableStep4, 1.0f, false);
}
void ADL2_HUD::EnableStep4()
{ 
	enableStep4 = true; 
	GetWorldTimerManager().SetTimer(m_TimerHandle, this, &ADL2_HUD::EnableStep5, 1.0f, false);
}
void ADL2_HUD::EnableStep5()
{
	enableStep5 = true;
	GetWorldTimerManager().SetTimer(m_TimerHandle, this, &ADL2_HUD::EnableStep6, 1.0f, false);
}
void ADL2_HUD::EnableStep6()
{
	enableStep6 = true;
	GetWorldTimerManager().SetTimer(m_TimerHandle, this, &ADL2_HUD::EnableStep7, 1.0f, false);
}
void ADL2_HUD::EnableStep7()
{
	enableStep7 = true;
}

void ADL2_HUD::DrawBackground()
{
	std::vector<PathGraphNode*>::iterator iter;
	for (iter = DungeonGenerator::getInstance()->getAreas().begin(); iter != DungeonGenerator::getInstance()->getAreas().end(); iter++)
	{
		Area* area = static_cast<Area*>(*iter);
		if (area)
		{
			FBox2D rect = area->getRect();
			DrawRect(FLinearColor::Gray, rect.Min.X*unit, rect.Min.Y*unit, rect.GetSize().X*unit, rect.GetSize().Y*unit);
		}
	}
}
void ADL2_HUD::DrawConnectAreas()
{
	//绘制连接区域
	std::vector<Area*>::iterator iter;
	for (iter = DungeonGenerator::getInstance()->getConnectedAreas().begin(); iter != DungeonGenerator::getInstance()->getConnectedAreas().end(); iter++)
	{
		Area* area = *iter;
		if (area)
		{
			FBox2D rect = area->getRect();
			if (enableStep5)
			{
				if (area->getAreaTypeMask() == EAreaTypeMaskEnum::ATME_MainPath)
					DrawRect(FLinearColor(FColor::Orange), rect.Min.X*unit, rect.Min.Y*unit, rect.GetSize().X*unit, rect.GetSize().Y*unit);
				else if (area->getAreaTypeMask() == EAreaTypeMaskEnum::ATME_SidePath)
					DrawRect(FLinearColor(FColor::Turquoise), rect.Min.X*unit, rect.Min.Y*unit, rect.GetSize().X*unit, rect.GetSize().Y*unit);
				else if (area->getAreaTypeMask() == EAreaTypeMaskEnum::ATME_BranchPath)
					DrawRect(FLinearColor(FColor::Cyan), rect.Min.X*unit, rect.Min.Y*unit, rect.GetSize().X*unit, rect.GetSize().Y*unit);
				else
					DrawRect(FLinearColor::Gray, rect.Min.X*unit, rect.Min.Y*unit, rect.GetSize().X*unit, rect.GetSize().Y*unit);
			}
			else
				DrawRect(FLinearColor::Gray, rect.Min.X*unit, rect.Min.Y*unit, rect.GetSize().X*unit, rect.GetSize().Y*unit);
		}
	}
}
void ADL2_HUD::DrawSplitLines()
{
	///绘制分割区域
	if (enableStep4)
	{
		std::vector<Area*>::iterator iter;
		for (iter = DungeonGenerator::getInstance()->getConnectedAreas().begin(); iter != DungeonGenerator::getInstance()->getConnectedAreas().end(); iter++)
		{
			Area* area = static_cast<Area*>(*iter);
			if (area)
			{
				FBox2D rect = area->getRect();
				DrawLine(rect.Min.X*unit, rect.Min.Y*unit, rect.Min.X*unit, rect.Max.Y*unit, FLinearColor::Green, 2.0f);
				DrawLine(rect.Min.X*unit, rect.Max.Y*unit, rect.Max.X*unit, rect.Max.Y*unit, FLinearColor::Green, 2.0f);
				DrawLine(rect.Max.X*unit, rect.Max.Y*unit, rect.Max.X*unit, rect.Min.Y*unit, FLinearColor::Green, 2.0f);
				DrawLine(rect.Max.X*unit, rect.Min.Y*unit, rect.Min.X*unit, rect.Min.Y*unit, FLinearColor::Green, 2.0f);
			}
		}
	}
	else
	{
		std::vector<PathGraphNode*>::iterator iter;
		for (iter = DungeonGenerator::getInstance()->getAreas().begin(); iter != DungeonGenerator::getInstance()->getAreas().end(); iter++)
		{
			Area* area = static_cast<Area*>(*iter);
			if (area)
			{
				FBox2D rect = area->getRect();
				DrawLine(rect.Min.X*unit, rect.Min.Y*unit, rect.Min.X*unit, rect.Max.Y*unit, FLinearColor::Green, 2.0f);
				DrawLine(rect.Min.X*unit, rect.Max.Y*unit, rect.Max.X*unit, rect.Max.Y*unit, FLinearColor::Green, 2.0f);
				DrawLine(rect.Max.X*unit, rect.Max.Y*unit, rect.Max.X*unit, rect.Min.Y*unit, FLinearColor::Green, 2.0f);
				DrawLine(rect.Max.X*unit, rect.Min.Y*unit, rect.Min.X*unit, rect.Min.Y*unit, FLinearColor::Green, 2.0f);
			}
		}
	}
	if (enableStep6)
	{
		std::vector<Area*>::iterator iter;
		for (iter = DungeonGenerator::getInstance()->getPivotalAreas().begin(); iter != DungeonGenerator::getInstance()->getPivotalAreas().end(); iter++)
		{
			Area* area = static_cast<Area*>(*iter);
			if (area)
			{
				FBox2D rect = area->getRect();
				DrawLine(rect.Min.X*unit, rect.Min.Y*unit, rect.Min.X*unit, rect.Max.Y*unit, FLinearColor(FColor::Blue), 2.0f);
				DrawLine(rect.Min.X*unit, rect.Max.Y*unit, rect.Max.X*unit, rect.Max.Y*unit, FLinearColor(FColor::Blue), 2.0f);
				DrawLine(rect.Max.X*unit, rect.Max.Y*unit, rect.Max.X*unit, rect.Min.Y*unit, FLinearColor(FColor::Blue), 2.0f);
				DrawLine(rect.Max.X*unit, rect.Min.Y*unit, rect.Min.X*unit, rect.Min.Y*unit, FLinearColor(FColor::Blue), 2.0f);
			}
		}
		for (iter = DungeonGenerator::getInstance()->getSpecialAreas().begin(); iter != DungeonGenerator::getInstance()->getSpecialAreas().end(); iter++)
		{
			Area* area = static_cast<Area*>(*iter);
			if (area)
			{
				FBox2D rect = area->getRect();
				DrawLine(rect.Min.X*unit, rect.Min.Y*unit, rect.Min.X*unit, rect.Max.Y*unit, FLinearColor(FColor::Magenta), 2.0f);
				DrawLine(rect.Min.X*unit, rect.Max.Y*unit, rect.Max.X*unit, rect.Max.Y*unit, FLinearColor(FColor::Magenta), 2.0f);
				DrawLine(rect.Max.X*unit, rect.Max.Y*unit, rect.Max.X*unit, rect.Min.Y*unit, FLinearColor(FColor::Magenta), 2.0f);
				DrawLine(rect.Max.X*unit, rect.Min.Y*unit, rect.Min.X*unit, rect.Min.Y*unit, FLinearColor(FColor::Magenta), 2.0f);
			}
		}
	}
}

void ADL2_HUD::DrawEntranceAreaAndExitArea()
{
	///绘制出入口
	Area* entranceArea = DungeonGenerator::getInstance()->getEntranceArea();
	if (entranceArea)
	{
		FBox2D rect = entranceArea->getRect();
		DrawRect(FLinearColor::Red, rect.Min.X*unit, rect.Min.Y*unit, rect.GetSize().X*unit, rect.GetSize().Y*unit);
	}
	Area* exitArea = DungeonGenerator::getInstance()->getExitArea();
	if (exitArea)
	{
		FBox2D rect = exitArea->getRect();
		DrawRect(FLinearColor::Green, rect.Min.X*unit, rect.Min.Y*unit, rect.GetSize().X*unit, rect.GetSize().Y*unit);
	}
	Area* branchArea = DungeonGenerator::getInstance()->getBranchArea();
	if (branchArea)
	{
		FBox2D rect = branchArea->getRect();
		DrawRect(FLinearColor::Yellow, rect.Min.X*unit, rect.Min.Y*unit, rect.GetSize().X*unit, rect.GetSize().Y*unit);
	}
}
void ADL2_HUD::DrawFinalMap()
{
	if (!m_pDungeonRoot || !m_pDungeonRoot->getCurrentDungeonNode())
		return;
	for (size_t i = 0; i < m_pDungeonRoot->getCurrentDungeonNode()->getMap().size(); i++)
	{
		Cell cell = m_pDungeonRoot->getCurrentDungeonNode()->getMap()[i];
		if (cell.getCellType() < ECellTypeEnum::CTE_StandardWall)
		{
			switch (cell.getAreaType())
			{
			case EAreaTypeEnum::ATE_Standard:
			case EAreaTypeEnum::ATE_Passage:
				DrawRect(FLinearColor::Gray, cell.getIndexX()*unit, cell.getIndexY()*unit, unit, unit);
				break;
			case EAreaTypeEnum::ATE_Tunnel:
				DrawRect(FLinearColor(FColor::Orange), cell.getIndexX()*unit, cell.getIndexY()*unit, unit, unit);
				break;
			case EAreaTypeEnum::ATE_Entrance:
				DrawRect(FLinearColor::Red, cell.getIndexX()*unit, cell.getIndexY()*unit, unit, unit);
				break;
			case EAreaTypeEnum::ATE_Exit:
				DrawRect(FLinearColor::Green, cell.getIndexX()*unit, cell.getIndexY()*unit, unit, unit);
				break;
			case EAreaTypeEnum::ATE_Branch:
				DrawRect(FLinearColor::Yellow, cell.getIndexX()*unit, cell.getIndexY()*unit, unit, unit);
				break;
			case EAreaTypeEnum::ATE_Pivotal:
				DrawRect(FLinearColor::Blue, cell.getIndexX()*unit, cell.getIndexY()*unit, unit, unit);
				break;
			case EAreaTypeEnum::ATE_Special:
				DrawRect(FLinearColor(FColor::Magenta), cell.getIndexX()*unit, cell.getIndexY()*unit, unit, unit);
				break;
			case EAreaTypeEnum::ATE_Unusual:
				DrawRect(FLinearColor(FColor::Purple), cell.getIndexX()*unit, cell.getIndexY()*unit, unit, unit);
				break;
			default:
				break;
			}
		}
		else if(cell.getCellType() == ECellTypeEnum::CTE_StandardWall || cell.getCellType() == ECellTypeEnum::CTE_PassageWall)
			DrawRect(FLinearColor(0.25f, 0.25f, 0.25f, 1.0f), cell.getIndexX()*unit, cell.getIndexY()*unit, unit, unit);
		else if (cell.getCellType() == ECellTypeEnum::CTE_Entrance)
		{
		}
		else if (cell.getCellType() == ECellTypeEnum::CTE_Exit)
		{
		}
		else if (cell.getCellType() == ECellTypeEnum::CTE_BranchExit)
		{
		}
		else if (cell.getCellType() >= ECellTypeEnum::CTE_StandardDoor && cell.getCellType() <= ECellTypeEnum::CTE_BossTreasuryDoor)
		{
			DrawRect(FLinearColor::Gray, cell.getIndexX()*unit, cell.getIndexY()*unit, unit, unit);
			if (cell.getCellType() == ECellTypeEnum::CTE_StandardDoor)
			{
				if (cell.getDirect() == EDirectEnum::DE_Left || cell.getDirect() == EDirectEnum::DE_Right)
					DrawLine((cell.getIndexX() + 0.5f)*unit, cell.getIndexY()*unit + 2.0f, (cell.getIndexX() + 0.5f)*unit, (cell.getIndexY() + 1)*unit, FLinearColor(FColor::Blue), 4.0f);
				else if (cell.getDirect() == EDirectEnum::DE_Forward || cell.getDirect() == EDirectEnum::DE_Backward)
					DrawLine(cell.getIndexX()*unit + 2.0f, (cell.getIndexY() + 0.5f)*unit, (cell.getIndexX() + 1)*unit, (cell.getIndexY() + 0.5f)*unit, FLinearColor(FColor::Blue), 4.0f);
			}
			else if (cell.getCellType() == ECellTypeEnum::CTE_HiddenDoor)
			{
				if (cell.getDirect() == EDirectEnum::DE_Left || cell.getDirect() == EDirectEnum::DE_Right)
					DrawRect(FLinearColor(0.2f, 0.2f, 0.2f, 1.0f), cell.getIndexX()*unit, cell.getIndexY()*unit, unit, unit);
				else if (cell.getDirect() == EDirectEnum::DE_Forward || cell.getDirect() == EDirectEnum::DE_Backward)
					DrawRect(FLinearColor(0.2f, 0.2f, 0.2f, 1.0f), cell.getIndexX()*unit, cell.getIndexY()*unit, unit, unit);
			}
			else if (cell.getCellType() == ECellTypeEnum::CTE_LockedDoor)
			{
				if (cell.getDirect() == EDirectEnum::DE_Left || cell.getDirect() == EDirectEnum::DE_Right)
					DrawLine((cell.getIndexX() + 0.5f)*unit, cell.getIndexY()*unit + 2.0f, (cell.getIndexX() + 0.5f)*unit, (cell.getIndexY() + 1)*unit, FLinearColor(FColor::Red), 4.0f);
				else if (cell.getDirect() == EDirectEnum::DE_Forward || cell.getDirect() == EDirectEnum::DE_Backward)
					DrawLine(cell.getIndexX()*unit + 2.0f, (cell.getIndexY() + 0.5f)*unit, (cell.getIndexX() + 1)*unit, (cell.getIndexY() + 0.5f)*unit, FLinearColor(FColor::Red), 4.0f);
			}
			else
			{
				if (cell.getDirect() == EDirectEnum::DE_Left || cell.getDirect() == EDirectEnum::DE_Right)
					DrawLine((cell.getIndexX() + 0.5f)*unit, cell.getIndexY()*unit + 2.0f, (cell.getIndexX() + 0.5f)*unit, (cell.getIndexY() + 1)*unit, FLinearColor(FColor::Magenta), 4.0f);
				else if (cell.getDirect() == EDirectEnum::DE_Forward || cell.getDirect() == EDirectEnum::DE_Backward)
					DrawLine(cell.getIndexX()*unit + 2.0f, (cell.getIndexY() + 0.5f)*unit, (cell.getIndexX() + 1)*unit, (cell.getIndexY() + 0.5f)*unit, FLinearColor(FColor::Magenta), 4.0f);
			}
		}
		else 
		{
			
		}
	}
}
void ADL2_HUD::DrawStatistics()
{
	if (!m_pDungeonRoot || !m_pDungeonRoot->getCurrentDungeonNode())
		return;
	int width = m_pDungeonRoot->getCurrentDungeonNode()->CellCountX + 1;
	int height = m_pDungeonRoot->getCurrentDungeonNode()->CellCountY + 1;

	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EDungeonStyle"), true);
	if (!EnumPtr) 
		return ;
	FString text = "Dungeon Style : " + EnumPtr->GetEnumName((int32)m_pDungeonRoot->getCurrentDungeonNode()->DungeonStyle);
	DrawText(text, FLinearColor(FColor::White), (width + 2)*unit, unit, nullptr, 2.0f);

	text = "Entrance Area count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getEntranceAreaCount());
	DrawText(text, FLinearColor::Red, (width + 2)*unit, unit*5);

	text = "Exit Area count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getExitAreaCount());
	DrawText(text, FLinearColor::Green, (width + 2)*unit, unit*7);

	text = "Branch Area count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getBranchAreaCount());
	DrawText(text, FLinearColor::Yellow, (width + 2)*unit, unit * 9);

	text = "Main Path Area count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getMainPathAreaCount());
	DrawText(text, FLinearColor(FColor::Orange), (width + 2)*unit, unit * 11);

	text = "Side Path Area count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getSidePathAreaCount());
	DrawText(text, FLinearColor(FColor::Turquoise), (width + 2)*unit, unit * 13);

	text = "Branch Path Area count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getBranchPathAreaCount());
	DrawText(text, FLinearColor(FColor::Cyan), (width + 2)*unit, unit * 15);

	text = "Secondary Area count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getSecondaryAreaCount());
	DrawText(text, FLinearColor::Black, (width + 2)*unit, unit * 17);

	text = "Pivotal Area Count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getPivotalAreaCount());
	DrawText(text, FLinearColor(FColor::Blue), (width + 2)*unit, unit * 19);

	text = "Special Area count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getSpecialAreaCount());
	DrawText(text, FLinearColor(FColor::Magenta), (width + 2)*unit, unit * 21);

	text = "Unusual Area count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getUnusualAreaCount());
	DrawText(text, FLinearColor(FColor::Purple), (width + 2)*unit, unit * 23);

	text = "Standard Area count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getStandardAreaCount());
	DrawText(text, FLinearColor(FColor::White), (width + 2)*unit, unit * 25);

	text = "Passage Area count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getPassageAreaCount());
	DrawText(text, FLinearColor(FColor::White), (width + 2)*unit, unit * 27);

	text = "Tunnel Area count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getTunnelAreaCount());
	DrawText(text, FLinearColor(FColor::White), (width + 2)*unit, unit * 29);

	text = "Standard Door count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getStandardDoorCount());
	DrawText(text, FLinearColor::Blue, (width + 2)*unit, unit * 31);
	text = "Locked Door count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getLockedDoorCount());
	DrawText(text, FLinearColor::Red, (width + 2)*unit, unit * 33);
	text = "Hidden Door count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getHiddenDoorCount());
	DrawText(text, FLinearColor(0.2f, 0.2f, 0.2f, 1.0f), (width + 2)*unit, unit * 35);
	text = "Special Door count : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->getSpecialDoorCount());
	DrawText(text, FLinearColor(FColor::Magenta), (width + 2)*unit, unit * 37);
}
void ADL2_HUD::DrawCircle(const FVector2D& Center, const FColor& Color, int32 NumSides, float Radius)
{
	if (IsCanvasValid_WarnIfNot())
	{
		FCanvasNGonItem NGonItem(Center, FVector2D(Radius, Radius), FMath::Clamp(NumSides, 3, 255), Color);
		NGonItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(NGonItem);
	}
}
void ADL2_HUD::DrawDungeonNode(UDungeonNodeComponent* node, const FVector2D& Center)
{
	FColor color;
	if (node->getVisitedType() == EVisitedType::VTE_NO)
		color = FColor::Blue;
	else if (node->getVisitedType() == EVisitedType::VTE_YES)
		color = FColor::Yellow;
	else
		color = FColor::Green;
	
	if (node->IsBossDungeonNode)
		color = FColor::Magenta;

	FVector2D leftCenter, rightCenter;
	if (node->getLeftNode())
	{
		leftCenter = FVector2D(Center.X - unit * 4, unit * 8 * node->getLeftNode()->NodeDepth);
		DrawLine(Center.X, Center.Y, leftCenter.X, leftCenter.Y, FLinearColor(FColor::Orange), 2.0f);
	}
	if (node->getRightNode())
	{
		rightCenter = FVector2D(Center.X + unit * 4, unit * 8 * node->getRightNode()->NodeDepth);
		DrawLine(Center.X, Center.Y, rightCenter.X, rightCenter.Y, FLinearColor(FColor::Cyan), 2.0f);
	}

	if (node == m_pDungeonRoot->getCurrentDungeonNode())
	{
		DrawCircle(Center, FColor::Red, 20, 2.5f*unit);
	}

	DrawCircle(Center, color, 20, 2*unit);

	if(node->getLeftNode())
		DrawDungeonNode(node->getLeftNode(), leftCenter);
	if(node->getRightNode())
		DrawDungeonNode(node->getRightNode(), rightCenter);

	
	DrawText(node->DungeonNodeName, FLinearColor::White, Center.X - unit*2.5f, Center.Y + unit*2.5f);
}
void ADL2_HUD::DrawDungeonTreeInfo()
{
	if (!m_pDungeonRoot || !m_pDungeonRoot->getCurrentDungeonNode())
		return;	
	FVector2D viewportSize;
	GEngine->GameViewport->GetViewportSize(viewportSize);
	int width = m_pDungeonRoot->getCurrentDungeonNode()->CellCountX + 1;
	int height = m_pDungeonRoot->getCurrentDungeonNode()->CellCountY + 1;

	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EDungeonType"), true);
	if (!EnumPtr)
		return;
	FString text = "Dungeon Type : " + EnumPtr->GetEnumName((int32)m_pDungeonRoot->DungeonType);
	DrawText(text, FLinearColor(FColor::White), (width + 2)*unit + viewportSize.X*0.35f, unit, nullptr, 2.0f);

	DrawText("Max Dungeon Depth : " + FString::FromInt(m_pDungeonRoot->MaxDungeonDepth), FLinearColor::Green, (width + 2)*unit + viewportSize.X*0.40f, unit * 5);
	if (m_pDungeonRoot->getCurrentDungeonNode())
		DrawText("Current Dungeon Depth : " + FString::FromInt(m_pDungeonRoot->getCurrentDungeonNode()->NodeDepth), FLinearColor::Yellow, (width + 2)*unit + viewportSize.X*0.40f, unit * 7);

	UDungeonNodeComponent* root = m_pDungeonRoot->getRootDungeonNode();
	if (root)
		DrawDungeonNode(root, FVector2D((width + 2)*unit + viewportSize.X*0.3f, unit * 8 * root->NodeDepth));
}
#endif // WITH_EDITOR
