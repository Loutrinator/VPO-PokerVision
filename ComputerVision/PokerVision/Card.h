#pragma once
#include "Image.h"
#include "CardValue.h"
class Card : public Image
{
public:
	Card(cv::Mat m, CardValue value):Image(m), cardValue(value){}
	CardValue cardValue;
};

