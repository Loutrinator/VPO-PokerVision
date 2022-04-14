#pragma once
#include <opencv2/opencv.hpp>
#include <string>
class Image
{
private:

	//Calcule des angles de l'image d'origine
	void init();
public:
	Image();
	Image(cv::Mat m);
	Image(cv::Mat m, const cv::Ptr<cv::ORB>& orb);

	std::vector<cv::Point2f> corners;
	cv::Mat mat;
	cv::Mat originalMat;
	std::vector<cv::KeyPoint> keypoints;
	cv::Mat descriptor;
	cv::Scalar color;
	std::vector<std::vector<cv::DMatch>> matches;
	std::vector<cv::DMatch> matchesGood;
	std::string name = "Image";
	cv::Point2f center;

	void detectAndCompute(const cv::Ptr<cv::ORB>& orb);

	void knnMatch(const cv::Ptr<cv::BFMatcher>& bfm, const Image& img, const float distance);

	cv::Mat convertToHsv();

	void Clipping(
		const Image& rightImg,
		cv::Mat& outImag,
		const int minGoodMatch = 10,
		const int thickness = 2);

	static void prepareImgs(const std::string& path,
		std::vector<Image>& leftImgs,
		const cv::Ptr<cv::ORB>& orb);
	static float getAngleBetween(const cv::Point2f a, const cv::Point2f b, const cv::Point2f c);

	void showImage(bool showPoints, bool showOriginal);

};