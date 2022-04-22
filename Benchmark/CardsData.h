#pragma once

#include "PointData.h"
#include <vector>
class CardsData
{
public:
	CardsData() = default;
	CardsData(int v,
		int c,
		int g,
		json jPoints);
	int rank;
	int suit;
	int group;
	std::vector<PointData> points;
};

