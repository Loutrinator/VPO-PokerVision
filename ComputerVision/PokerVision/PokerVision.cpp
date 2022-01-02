#include "PokerVision.h"

PokerVision::PokerVision()
{
	cardOrb = cv::ORB::create(2000, 1.2, 8, 1, 2);
	bfm = cv::BFMatcher::create();
}

void PokerVision::setCardsDataset(const cv::Mat& cardsFile, int width, int height)
{
	cv::Mat tmp = cardsFile.clone();
	brightnessContrast(tmp, 1.3, -15);
	//increaseReadability(tmp);
	//
	
	cardWidth = floor(cardsFile.cols / width);
	cardHeight = floor(cardsFile.rows / height);
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {

			//on calcule la zone à lire pour récupérer notre image
			float minX = x * cardWidth + borderOffset;
			float minY = y * cardHeight + borderOffset;
			float maxX = (x + 1) * cardWidth - borderOffset;
			float maxY = (y + 1) * cardHeight - borderOffset;
			
			if(pipOnly){
				float pipRoiCoeffx = 1.3f / 8.0f;
				float pipRoiCoeffy = 3.3f / 11.0f;
				maxX = minX + (maxX - minX) * pipRoiCoeffx;
				maxY = minY + (maxY - minY) * pipRoiCoeffy;
			}

			cv::Rect cardROI(cv::Point2f(minX, minY), cv::Point2f(maxX, maxY));
			CardValue value(CardRank(14 - y), CardSuit(x));
			Card card(tmp(cardROI), value, cardOrb);
			cv::waitKey(0);
			cards.push_back(card);
		}
	}
}

void PokerVision::findCards(Image& img, bool printRes)
{
	std::string desc = "Detected hand: (";
	int cardsFound = 0;
	for (Card card : cards) {
		bfm = cv::BFMatcher::create();
		if (bfm != nullptr) {
			card.knnMatch(bfm, img, 0.75);
		}
		std::cout << "Matches : " << card.matchesGood.size() << std::endl;
		if (card.matchesGood.size() > nbPointsToMatch) {
			std::cout << "Found " << card.cardValue.ToString() << std::endl;
			if (cardsFound > 0) desc += ", ";
			desc += card.cardValue.ToShortString();
			if (showCards) {
				card.showImage(showKeypoints);
			}
			cardsFound++;
		}
	}
	desc += ")";
	std::cout << desc << std::endl;
	if (printRes) {
		int font = cv::FONT_HERSHEY_SIMPLEX;
		cv::Point2f bottomLeftCornerOfText(25, 25);
		float fontScale = 0.7;
		cv::Scalar fontColor(0, 0, 255);
		int thickness = 1;
		int lineType = 2;
		cv::putText(img.mat, desc, bottomLeftCornerOfText, font, fontScale, fontColor, thickness, lineType);
	}
}

void PokerVision::increaseReadability(cv::Mat& img)
{
	cv::Mat tmp;
	cv::cvtColor(img, tmp, cv::COLOR_BGR2GRAY);
	//cv::threshold(tmp, tmp, 160, 255, cv::THRESH_BINARY);
	cv::adaptiveThreshold(tmp, tmp, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 5, 12);
	img = tmp;
}

void PokerVision::brightnessContrast(cv::Mat& img, double contrast, int brightness)
{

	cv::Mat tmp = cv::Mat::zeros(img.size(), img.type());
	for (int y = 0; y < img.rows; y++) {
		for (int x = 0; x < img.cols; x++) {
			for (int c = 0; c < img.channels(); c++) {
				tmp.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(contrast * img.at<cv::Vec3b>(y, x)[c] + brightness);
			}
		}
	}
	img = tmp;
}
