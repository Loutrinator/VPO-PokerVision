#include "CardGroup.h"


void CardGroup::setColors(cv::Scalar roi, cv::Scalar center)
{
	for (Card& card : cards) {
		card.roiColor = roi;
		card.centerColor = center;
	}
}
