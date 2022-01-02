#include "Card.h"

Card::Card(cv::Mat m, CardValue value, cv::Ptr<cv::ORB>& orb) :Image(m), cardValue(value) {
	detectAndCompute(orb);
	name = value.ToString();
}