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
	bool IsValid;
	int rank;
	int suit;
	int group;
	std::vector<PointData> points;
	std::vector<CardsData> groupedCards;

	bool isSameCard(CardsData other) {

		if (!(other.rank == rank && other.suit == suit))
			return false;
		if (other.points.size() != points.size())
			return false;

		for (size_t i = 0; i < points.size(); i++)
		{
			if (!points[i].isSamePoint(other.points[i]))
			{
				return false;
			}
		}
		return true;
	}
};

