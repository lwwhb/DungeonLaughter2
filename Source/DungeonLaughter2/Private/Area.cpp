// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "Area.h"

Area::Area()
{
	m_AreaType = EAreaTypeEnum::ATE_Unknown;
	m_AreaTypeMask = EAreaTypeMaskEnum::ATME_Unknown;
	m_nWeight = 1;
}

Area::~Area()
{
}

void Area::addNeigbourArea(Area* other)
{
	if (!other)
		return;
	FBox2D rect = getIntersectRect(other);
	if ((rect.GetSize().X == 0 && rect.GetSize().Y >= 3) || (rect.GetSize().Y == 0 && rect.GetSize().X >= 3))
	{
		m_Neigbours.push_back(other);
		other->m_Neigbours.push_back(this);
	}
}
void Area::connectArea(Area* other)
{
	if (!other)
		return;
	if (m_ConnectedAreas.find(other) == m_ConnectedAreas.end()) {
		m_ConnectedAreas[other] = nullptr;
		other->m_ConnectedAreas[this] = nullptr;
	}
}

FBox2D Area::getRect() const
{
	return m_Rect;
}
void Area::setRect(const FBox2D& rect)
{
	m_Rect = rect;
}

EAreaTypeEnum Area::getAreaType() const
{
	return m_AreaType;
}
void Area::setAreaType(EAreaTypeEnum type)
{
	m_AreaType = type;
}
EAreaTypeMaskEnum Area::getAreaTypeMask() const
{
	return m_AreaTypeMask;
}
void Area::setAreaTypeMask(EAreaTypeMaskEnum mask)
{
	m_AreaTypeMask = mask;
}

FBox2D Area::getIntersectRect(Area* other)
{
	int minX = std::max(m_Rect.Min.X, other->getRect().Min.X);
	int minY = std::max(m_Rect.Min.Y, other->getRect().Min.Y);
	int maxX = std::min(m_Rect.Max.X, other->getRect().Max.X);
	int maxY = std::min(m_Rect.Max.Y, other->getRect().Max.Y);
	return FBox2D(FVector2D(minX, minY), FVector2D(maxX, maxY));
}