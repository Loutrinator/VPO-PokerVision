#include "PointData.h"

PointData::PointData(int vx, int vy)
{
	x = vx;
	y = vy;
}

json PointData::to_json()
{
	json j;
	j["x"] = x;
	j["y"] = y;
	return  j;
}
