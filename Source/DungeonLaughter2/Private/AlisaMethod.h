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
	static std::shared_ptr<AlisaMethod> create(double percent1, ...);
	static std::shared_ptr<AlisaMethod> createWithPercentArray(std::vector<double>& percentArray);

	int getRandomIndex();
	void printProbArray();
	void printAlisaArray();
protected:
	bool initWithPercentVariableList(double percent1, va_list args);
	bool initWithPercentArray(std::vector<double>& percentArray);
private:
	std::vector<double>      m_probArray;
	std::vector<int>        m_alisaArray;
};
