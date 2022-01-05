#pragma once
#include <vector>
#include "Card.h"
class CardGroup
{
public:
	void setColors(cv::Scalar roi, cv::Scalar center);
	std::vector<Card> cards;
	Card& operator[](int i) { return cards[i]; }
private:
};

