// PokerVision.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include <string>
#include <math.h>

#include "CardValue.h"
#include "Card.h"

int main(int argc, char** argv)
{
	std::cout << "RONAN " << std::endl;
	std::string windowName = "card";
	bool BnWInputs = true;

	// Chargement des images
	cv::Mat cards = cv::imread("resources/cartes.jpg");
	if (cards.empty()){
		std::cout << "Could not open or find the image" << std::endl;
		system("pause");
		return -1;
	}
	
	int cardsWidthCount = 4;
	int cardsHeightCount = 13;

	int cardWidth = floor(cards.cols / cardsWidthCount);
	int cardHeight = floor(cards.rows / cardsHeightCount);
	std::cout << "source file width: " + cards.cols << std::endl;
	std::cout << "source file height: " + cards.rows << std::endl;
	std::cout << "card width: " + cardWidth << std::endl;
	std::cout << "card height: " + cardHeight << std::endl;

	float borderOffset = 7; //utilisé pour ignorer les bordures des cartes et ne scanner que leur contenu
	std::vector<std::vector<Card>> cardList;


	for (int x = 0; x < cardsWidthCount; ++x) {
		std::vector<Card> cardsForSuit;
		for (int y = 0; y < cardsHeightCount; ++y) {

			//on calcule la zone à lire pour récupérer notre image
			cv::Rect cardROI(cv::Point2f(x* cardWidth + borderOffset, y * cardHeight + borderOffset), cv::Point2f((x+1) * cardWidth - borderOffset, (y + 1) * cardHeight - borderOffset));
		
			Card card(cards(cardROI), CardValue(CardRank(14 - y), CardSuit(x)));
			
			cardsForSuit.push_back(card);
		}
		cardList.push_back(cardsForSuit);
	}

	std::cout << cardList.size() << std::endl;
	cv::imshow("cards", cards);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}