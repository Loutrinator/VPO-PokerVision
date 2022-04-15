#pragma once
#include "CardsData.h"
#include <opencv2/opencv.hpp>
#include <vector>
using json = nlohmann::json;
class ImageData
{
public:
	std::vector<CardsData> cards;
	ImageData() = default;
	void addCard(int v, int c, int g, std::vector<cv::Point> ps);

	json to_json();
};

