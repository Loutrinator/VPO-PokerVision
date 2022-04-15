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
	int rank;
	int suit;
	int group;
	std::vector<cv::Point> pointsRaw;
	std::vector<PointData> points;
	json to_json();
};

