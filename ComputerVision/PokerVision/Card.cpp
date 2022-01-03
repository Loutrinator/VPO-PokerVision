#include "Card.h"

Card::Card(cv::Mat m, CardValue value, cv::Ptr<cv::ORB>& orb) :Image(m), cardValue(value) {
	detectAndCompute(orb);
	name = value.ToString();
	corners = std::vector<cv::Point2f>(4);

	int cornerOffset = 7;

	if (!mat.empty() && mat.cols > 0 && mat.rows > 0)
	{
		corners[0] = cv::Point2f(0 - cornerOffset, 0 - cornerOffset);
		corners[1] = cv::Point2f((float)mat.cols + cornerOffset, 0 - cornerOffset);
		corners[2] = cv::Point2f((float)mat.cols + cornerOffset, (float)mat.rows + cornerOffset);
		corners[3] = cv::Point2f(0 - cornerOffset, (float)mat.rows + cornerOffset);
	}
}