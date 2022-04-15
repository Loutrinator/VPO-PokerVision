#include "ImageData.h"

void ImageData::addCard(const int v, const int c, const int g, std::vector<cv::Point> ps)
{
	cards.push_back(CardsData(v, c, g, ps));
}
