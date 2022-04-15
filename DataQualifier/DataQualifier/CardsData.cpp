#include "CardsData.h"

CardsData::CardsData(int v, int c, int g, std::vector<cv::Point> ps)
{
	value = v;
	color = c;
	group = g;
	pointsRaw = ps;
	for (cv::Point& p : ps) {
		PointData tmp(p.x, p.y);
		points.push_back(tmp);
	}
}
