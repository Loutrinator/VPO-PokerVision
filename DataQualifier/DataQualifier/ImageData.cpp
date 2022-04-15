#include "ImageData.h"

void ImageData::addCard(const int v, const int c, const int g, std::vector<cv::Point> ps)
{
	cards.push_back(CardsData(v, c, g, ps));
}

json ImageData::to_json()
{
	json j;
	std::vector<json> jCards;
	for (auto& c : cards)
	{
		jCards.push_back(c.to_json());
	}
	j["cards"] = jCards;
	return j;
}