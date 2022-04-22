#include "ImageData.h"

ImageData::ImageData(json js)
{
	for (auto& j : js["cards"])
	{
		cards.push_back(CardsData(j["rank"], j["suit"], j["group"], j["points"]));
	}
	viewStraightness = js["viewStraightness"];
}


