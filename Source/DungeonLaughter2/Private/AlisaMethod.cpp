// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonLaughter2.h"
#include "AlisaMethod.h"
#include <deque>
#include <string>

std::shared_ptr<AlisaMethod> AlisaMethod::create(double percent1, ...)
{
	va_list params;
	va_start(params, percent1);
	std::shared_ptr<AlisaMethod> ret(new (std::nothrow) AlisaMethod());
	if (ret)
		ret.get()->initWithPercentVariableList(percent1, params);
	va_end(params);
	return ret;
}
std::shared_ptr<AlisaMethod> AlisaMethod::createWithPercentArray(std::vector<double>& percentArray)
{
	std::shared_ptr<AlisaMethod> ret(new (std::nothrow) AlisaMethod());
	if (ret)
		ret.get()->initWithPercentArray(percentArray);
	return ret;
}
bool AlisaMethod::initWithPercentVariableList(double percent1, va_list args)
{
	std::vector<double> percentArray;
	percentArray.push_back(percent1);
	double total = percent1;
	while (1)
	{
		float nextPercent = va_arg(args, double);
		if (nextPercent < 1.0 && nextPercent >= 0.0)
		{
			total += nextPercent;
			percentArray.push_back(nextPercent);
		}
		else
			break;
	}
    if(FMath::Abs(total - 1.0) > 0.00001)
		UE_LOG(LogTemp, Fatal, TEXT("The sum of those percents must be 1.0f! The value is %f"), total);
	
	return initWithPercentArray(percentArray);
}
bool AlisaMethod::initWithPercentArray(std::vector<double>& percentArray)
{
	std::deque<double> small, large;
	for (int i = 0; i < percentArray.size(); ++i) {
		percentArray[i] *= percentArray.size();
		if (percentArray[i] < 1.0)
			small.push_back(i);
		else
			large.push_back(i);
	}

	m_probArray.resize(percentArray.size());
	m_alisaArray.resize(percentArray.size());
	while (small.size() != 0 && large.size() != 0) {
		float s_index = small.front();
		small.pop_front();
		float l_index = large.front();
		large.pop_front();
		m_probArray[s_index] = percentArray[s_index];
		m_alisaArray[s_index] = l_index;
		percentArray[l_index] -= 1.0 - percentArray[s_index];
		if (percentArray[l_index] < 1.0)
			small.push_back(l_index);
		else
			large.push_back(l_index);
	}

	while (!small.empty()) {
		m_probArray[small.front()] = 1.0;
		small.pop_front();
	}
	while (!large.empty()) {
		m_probArray[large.front()] = 1.0;
		large.pop_front();
	}
	return true;
}
int AlisaMethod::getRandomIndex()
{
	int index = FMath::RandRange(0, (int)(m_probArray.size() - 1));
	return FMath::RandRange(0.0f,1.0f) < m_probArray[index] ? index : m_alisaArray[index];
}
void AlisaMethod::printProbArray()
{
	FString text = "ProbArray: ";
	for (int i = 0; i < m_probArray.size(); ++i) {
		text += FString::SanitizeFloat(m_probArray[i]);
	}
	text += "\n";
	UE_LOG(LogTemp, Display, TEXT("%s"), std::string(TCHAR_TO_UTF8(*text)).c_str());
}
void AlisaMethod::printAlisaArray()
{
	FString text = "AlisaArray: ";
	for (int i = 0; i < m_alisaArray.size(); ++i) {
		text += FString::SanitizeFloat(m_alisaArray[i]);
	}
	UE_LOG(LogTemp, Display, TEXT("%s"), std::string(TCHAR_TO_UTF8(*text)).c_str());
}
///test
//std::shared_ptr<AlisaMethod> am = AlisaMethod::create(0.1,0.2,0.3,0.4);
//if(am)
//{
//    am.get()->printProbArray();
//    am.get()->printAlisaArray();
//    int a=0,b=0,c=0,d = 0;
//    for (int i = 0; i<10000; ++i) {
//        int index = am.get()->getRandomIndex();
//        if(index == 0)
//            a++;
//        if(index == 1)
//            b++;
//        if(index == 2)
//            c++;
//        if(index == 3)
//            d++;
//    }
//    UE_LOG(LogTemp, Log, TEXT("random a = %d, b= %d, c= %d d = %d"), a, b ,c ,d);
//}
