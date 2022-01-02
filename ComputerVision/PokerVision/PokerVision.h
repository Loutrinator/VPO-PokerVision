#pragma once
#include "Card.h"
class PokerVision
{
public:
	PokerVision();
	void setCardsDataset(const cv::Mat& cardsFile, int width, int height);
	void findCards(Image& img, bool printRes);
	void increaseReadability(cv::Mat& img);
	void brightnessContrast(cv::Mat& img, double contrast, int brightness);

	std::vector<Card> cards;
	cv::Ptr<cv::ORB> cardOrb;
	cv::Ptr<cv::BFMatcher> bfm;
	float borderOffset = 7; //utilisé pour ignorer les bordures des cartes et ne scanner que leur contenu
	bool showCards = false;
	bool showKeypoints = false;
	bool pipOnly = false;
	int nbPointsToMatch = 25;
private:
	int cardWidth = 0;
	int cardHeight = 0;
};

