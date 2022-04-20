#include "ImageData.h"

void ImageData::addCard(const int v, const int c, const int g, std::vector<cv::Point> ps)
{
	cards.push_back(CardsData(v, c, g, ps));
}

void ImageData::CalculateViewStraightness()
{
	for (auto& c : cards)
	{
		float moyDif = 0.f;
		moyDif += std::abs(getAngleBetween(c.pointsRaw[0], c.pointsRaw[1], c.pointsRaw[2]) - 90.f);
		moyDif += std::abs(getAngleBetween(c.pointsRaw[1], c.pointsRaw[2], c.pointsRaw[3]) - 90.f);
		moyDif += std::abs(getAngleBetween(c.pointsRaw[2], c.pointsRaw[3], c.pointsRaw[0]) - 90.f);
		moyDif += std::abs(getAngleBetween(c.pointsRaw[3], c.pointsRaw[0], c.pointsRaw[1]) - 90.f);
		moyDif /= 4;//on a donc une valeur qui varie de 0 à 90
		viewStraightness += 1 - (moyDif / 90); // straight = 1, nimp = 0 
	}
	viewStraightness /= cards.size();
}
float ImageData::getAngleBetween(const cv::Point2f a, const cv::Point2f b, const cv::Point2f c)
{
	cv::Point2f ab = { b.x - a.x, b.y - a.y };
	cv::Point2f cb = { b.x - c.x, b.y - c.y };

	float dot = (ab.x * cb.x + ab.y * cb.y); // dot product
	float cross = (ab.x * cb.y - ab.y * cb.x); // cross product

	float alpha = atan2(cross, dot);

	return std::abs((int)floor(alpha * 180.f / 3.141592654 + 0.5));
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
	j["viewStraightness"] = viewStraightness;
	return j;
}