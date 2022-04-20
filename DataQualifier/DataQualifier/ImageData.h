#pragma once
#include "CardsData.h"
#include <opencv2/opencv.hpp>
#include <vector>
using json = nlohmann::json;
class ImageData
{
public:
	float viewStraightness = 0.f;
	std::vector<CardsData> cards;
	ImageData() = default;
	void addCard(int v, int c, int g, std::vector<cv::Point> ps);
	void CalculateViewStraightness();
	float getAngleBetween(const cv::Point2f a, const cv::Point2f b, const cv::Point2f c);
	json to_json();
};

