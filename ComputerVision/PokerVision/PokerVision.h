#pragma once
#include "Card.h"
#include "CardGroup.h"
class PokerVision
{
public:
	PokerVision(bool pip, int nbMatch, float angleMargin);
	void setCardsDataset(const cv::Mat& cardsFile, int width, int height);
	void findCards(Image& img, bool enableLogs);
	void removeOverlapingImages(float minDist);
	void groupCards(float minDist);

	cv::Point2f getBarrycenter(std::vector<cv::Point2f> points);
	
	void divide(cv::Mat& img, cv::Vec3b rgb);
	void brightnessContrast(cv::Mat& img, double contrast, int brightness);

	void showImage(cv::Mat& img, std::string name, int width);
	
	void showResult(const Image& img, bool showProcessedImage, bool showCards, bool showPoints, bool showText, bool showROI, bool showBarrycenters);
	std::vector<Card> cards;
	std::vector<Card> foundCards;
	std::vector<CardGroup> cardGroups;
	cv::Ptr<cv::ORB> cardOrb;
	cv::Ptr<cv::BFMatcher> bfm;
	float borderOffset = 7; //utilisé pour ignorer les bordures des cartes et ne scanner que leur contenu
	bool pipOnly = false;
	int nbPointsToMatch = 25;
private:
	int cardWidth = 0;
	int cardHeight = 0;
	float minAngle = 45;
	float maxAngle = 135;

	bool angleIsValid(float angle);
	cv::Scalar HSV2BGR(float h, float s, float v);
	cv::Scalar BGR2HSV(float b, float g, float r);
	std::vector<std::vector<int>> getGroupCardIdsByDistance(float dist);
};

