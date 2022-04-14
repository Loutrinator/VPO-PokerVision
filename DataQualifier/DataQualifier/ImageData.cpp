#include "ImageData.h"

ImageData::ImageData(int v, int c, int g, cv::Point& tl, cv::Point& tr, cv::Point& bl, cv::Point& br)
{

	value = v;
	color = c;
	group = g;

	topLeft = PointData(tl.x, tl.y);
	topRight = PointData(tr.x, tr.y);
	bottomLeft = PointData(bl.x, bl.y);
	bottomRight = PointData(br.x, br.y);
}
