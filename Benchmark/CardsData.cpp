#include "CardsData.h"

CardsData::CardsData(int v, int c, int g, json jPoints)
{
	rank = v;
	suit = c;
	group = g;
	for (auto& p : jPoints)
	{
		points.push_back(PointData(p["x"], p["y"]));
	}
}
