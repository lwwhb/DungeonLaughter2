// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "DL2_HUD.h"
#include "../Private/DungeonGenerator.h"
#include "../Private/Area.h"


void ADL2_HUD::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	unit = DungeonGenerator::getInstance()->getCellUnit();
	enableStep1 = enableStep2 = enableStep3 = enableStep4 = enableStep5 = enableStep6 = false;
	GetWorldTimerManager().SetTimer(m_TimerHandle, this, &ADL2_HUD::EnableStep1, 1.0f, false);
#endif // WITH_EDITOR
}
void ADL2_HUD::DrawHUD()
{
	Super::DrawHUD();
#if WITH_EDITOR
	if (enableStep1)
	{
		if (enableStep4)
			DrawConnectAreas();
		else
			DrawBackground();
	}
	if (enableStep3)
		DrawEntranceAreaAndExitArea();
	if (enableStep2)
		DrawSplitLines();

	DrawStatistics();
#endif // WITH_EDITOR
}

#if WITH_EDITOR
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
void ADL2_HUD::DrawStatistics()
{
	int width = DungeonGenerator::getInstance()->getWidth();
	int height = DungeonGenerator::getInstance()->getHeight();

	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EDungeonStyle"), true);
	if (!EnumPtr) 
		return ;
	FString text = "Dungeon Style : " + EnumPtr->GetEnumName((int32)DungeonGenerator::getInstance()->getDungeonStyle());
	DrawText(text, FLinearColor(FColor::White), (width + 2)*unit, unit, nullptr, 2.0f);

	Area* entranceArea = DungeonGenerator::getInstance()->getEntranceArea();
	if (entranceArea)
		DrawText("Entrance Area count : 1", FLinearColor::Red, (width + 2)*unit, unit*5);
	else
		DrawText("Entrance Area count : 0", FLinearColor::Red, (width + 2)*unit, unit*5);

	Area* exitArea = DungeonGenerator::getInstance()->getExitArea();
	if (exitArea)
		DrawText("Exit Area count : 1", FLinearColor::Green, (width + 2)*unit, unit*7);
	else
		DrawText("Exit Area count : 0", FLinearColor::Green, (width + 2)*unit, unit*7);

	Area* branchArea = DungeonGenerator::getInstance()->getBranchArea();
	if (branchArea)
		DrawText("Branch Area count : 1", FLinearColor::Yellow, (width + 2)*unit, unit * 9);
	else
		DrawText("Branch Area count : 0", FLinearColor::Yellow, (width + 2)*unit, unit * 9);

	text = "Main Path Area count : " + FString::FromInt(DungeonGenerator::getInstance()->getMainPathAreasCount());
	DrawText(text, FLinearColor(FColor::Orange), (width + 2)*unit, unit * 11);

	text = "Side Path Area count : " + FString::FromInt(DungeonGenerator::getInstance()->getSidePathAreasCount());
	DrawText(text, FLinearColor(FColor::Turquoise), (width + 2)*unit, unit * 13);

	text = "Branch Path Area count : " + FString::FromInt(DungeonGenerator::getInstance()->getBranchPathAreasCount());
	DrawText(text, FLinearColor(FColor::Cyan), (width + 2)*unit, unit * 15);

	text = "Secondary Area count : " + FString::FromInt(DungeonGenerator::getInstance()->getSecondaryAreasCount());
	DrawText(text, FLinearColor::Black, (width + 2)*unit, unit * 17);

	text = "Pivotal Area Count : " + FString::FromInt(DungeonGenerator::getInstance()->getPivotalAreasCount());
	DrawText(text, FLinearColor(FColor::Blue), (width + 2)*unit, unit * 19);

	text = "Special Area count : " + FString::FromInt(DungeonGenerator::getInstance()->getSpecialAreaCount());
	DrawText(text, FLinearColor(FColor::Magenta), (width + 2)*unit, unit * 21);

	text = "Standard Area count : " + FString::FromInt(DungeonGenerator::getInstance()->getStandardAreaCount());
	DrawText(text, FLinearColor(FColor::White), (width + 2)*unit, unit * 23);

	text = "Passage Area count : " + FString::FromInt(DungeonGenerator::getInstance()->getPassageAreaCount());
	DrawText(text, FLinearColor(FColor::White), (width + 2)*unit, unit * 25);

	text = "Tunnel Area count : " + FString::FromInt(DungeonGenerator::getInstance()->getTunnelAreaCount());
	DrawText(text, FLinearColor(FColor::White), (width + 2)*unit, unit * 27);
}
#endif // WITH_EDITOR
