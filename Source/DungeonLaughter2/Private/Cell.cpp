// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "Cell.h"

CellInfo::CellInfo()
{
	m_nIndexX		= -1;
	m_nIndexY		= -1;
	m_nDepthZ		= 0;
	m_Type			= ECellTypeEnum::CTE_EmptyCell;
	m_AreaType		= EAreaTypeEnum::ATE_Unknown;
	m_AreaTypeMask	= EAreaTypeMaskEnum::ATME_Unknown;
	m_Direct		= EDirectEnum::DE_Unknown;
	m_nTypeIndex	= 0;
}

CellInfo::~CellInfo()
{
}
