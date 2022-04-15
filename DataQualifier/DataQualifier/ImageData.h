#pragma once
#include "CardsData.h"
#include <opencv2/opencv.hpp>
#include <vector>
class ImageData
{
public:
	std::vector<CardsData> cards;
	ImageData() = default;
	void addCard(int v, int c, int g, std::vector<cv::Point> ps);
};

