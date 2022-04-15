#pragma once

#include "PointData.h"
#include <opencv2/opencv.hpp>
#include <vector>
class CardsData
{
public:
	CardsData() = default;
	CardsData(int v,
		int c,
		int g,
		std::vector<cv::Point> ps);
	int value;
	int color;
	int group;
	std::vector<PointData> points;
};

