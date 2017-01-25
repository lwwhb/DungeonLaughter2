// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
#include<vector>
#include<queue>
class PathGraphNode
{
public:
	PathGraphNode();
	~PathGraphNode();

public:
	virtual int getDistance() const;
	virtual void setDistance(int distance);

	virtual int getWeight() const;
	virtual void setWeight(int weigit);

	const std::vector<PathGraphNode*>& getNeigbours();
protected:
	int     m_nDistance;
	int     m_nWeight;
	std::vector<PathGraphNode*>     m_Neigbours;
};
class PathGraph
{
public:
	static void setWeight(const std::vector<PathGraphNode*>& nodes, int value);
	static void buildDistanceMap(const std::vector<PathGraphNode*>& nodes, PathGraphNode* focus);
	static std::vector<PathGraphNode*> buildPath(const std::vector<PathGraphNode*>& nodes, PathGraphNode* from, PathGraphNode* to);
};