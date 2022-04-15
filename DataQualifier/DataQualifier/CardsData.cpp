#include "CardsData.h"

CardsData::CardsData(int v, int c, int g, std::vector<cv::Point> ps)
{
	rank = v;
	suit = c;
	group = g;
	pointsRaw = ps;
	for (cv::Point& p : ps) {
		PointData tmp(p.x, p.y);
		points.push_back(tmp);
	}
}

json CardsData::to_json()
{
	json j;
	j["rank"] = rank;
	j["suit"] = suit;
	j["group"] = group;
	std::vector<json> jPoints;
	for (auto& c : points)
		jPoints.push_back(c.to_json());

	j["points"] = jPoints;
	return j;
}
