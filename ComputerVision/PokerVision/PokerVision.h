#pragma once
#include "Card.h"
class PokerVision
{
public:
	PokerVision(bool pip, int nbMatch, float angleMargin);
	void setCardsDataset(const cv::Mat& cardsFile, int width, int height);
	void findCards(Image& img, bool enableLogs);
	void increaseReadability(cv::Mat& img);
	void brightnessContrast(cv::Mat& img, double contrast, int brightness);
	void showResult(const Image img, bool showCards, bool showPoints, bool showText, bool showROI, bool showBarrycenters);
	std::vector<Card> cards;
	std::vector<Card> foundCards;
	cv::Ptr<cv::ORB> cardOrb;
	cv::Ptr<cv::BFMatcher> bfm;
	float borderOffset = 7; //utilisé pour ignorer les bordures des cartes et ne scanner que leur contenu
	bool pipOnly = false;
	int nbPointsToMatch = 25;
private:
	int cardWidth = 0;
	int cardHeight = 0;
	float minAngle = 70;
	float maxAngle = 120;

	bool angleIsValid(float angle);
};

