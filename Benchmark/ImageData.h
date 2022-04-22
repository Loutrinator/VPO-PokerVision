#pragma once
#include "CardsData.h"
#include <vector>
using json = nlohmann::json;
class ImageData
{
public:
	float viewStraightness = 0.f;
	std::vector<CardsData> cards;
	ImageData() = default;
	ImageData(json j);
};

