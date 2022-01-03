#pragma once
#include "Image.h"
#include "CardValue.h"
#include <opencv2/opencv.hpp>
class Card : public Image
{
public:
	Card(cv::Mat m, CardValue value);
	CardValue cardValue;
	std::vector<cv::Point2f> gameCorners; //the corners of the card on the game board
	cv::Mat homographyMatrix;
};

