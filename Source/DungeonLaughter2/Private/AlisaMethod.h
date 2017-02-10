// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
#include <memory>
#include <vector>
class AlisaMethod
{
public:
	static std::shared_ptr<AlisaMethod> create(float percent1, ...);
	static std::shared_ptr<AlisaMethod> createWithPercentArray(std::vector<float>& percentArray);

	int getRandomIndex();
	void printProbArray();
	void printAlisaArray();
protected:
	bool initWithPercentVariableList(float percent1, va_list args);
	bool initWithPercentArray(std::vector<float>& percentArray);
private:
	std::vector<float>      m_probArray;
	std::vector<int>        m_alisaArray;
};
