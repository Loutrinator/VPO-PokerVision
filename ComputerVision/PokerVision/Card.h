#pragma once
#include "Image.h"
#include "CardValue.h"
#include <opencv2/opencv.hpp>
class Card : public Image
{
public:
	Card(cv::Mat m, CardValue value, cv::Ptr<cv::ORB>& orb);
	CardValue cardValue;
};

