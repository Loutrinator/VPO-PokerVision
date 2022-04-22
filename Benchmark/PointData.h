#pragma once
#include "nlohmann/json.hpp"
using json = nlohmann::json;
class PointData
{
public:
	PointData() = default;
	PointData(int vx, int vy);
	int x, y;
	json to_json();
};

