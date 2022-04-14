#pragma once
#include "PointData.h"
#include <opencv2/opencv.hpp>
class ImageData
{
public:
	ImageData() = default;
	ImageData(int v,
		int c,
		int g,
		cv::Point& tl,
		cv::Point& tr,
		cv::Point& bl,
		cv::Point& br);
	int value;
	int color;
	int group;
	PointData topLeft;
	PointData topRight;
	PointData bottomLeft;
	PointData bottomRight;
};

