#pragma once
#include <string>
class Config
{
public:
	Config(std::string configName);
	bool enableLogs = false;//true
	bool showResult = false;//true
	bool pipOnly = false;//false
	int nbPointsToMatch = 0;//10
	int angleTolerance = 0;//5
	int imageOrbMaxPointCount = 0;//50000
	int cardOrbMaxPointCount = 0;//2000
	int overlapDistThreshold = 0;//150
	int groupingDistance = 0;//300
};

