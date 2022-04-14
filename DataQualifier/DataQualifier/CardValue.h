#pragma once
#include <string>
#include <iostream>

enum CardSuit {
	Clubs = 0,
	Spades = 1,
	Diamonds = 2,
	Hearts = 3
};

enum CardRank {
	Two = 2,
	Three = 3,
	Four = 4,
	Five = 5,
	Six = 6,
	Seven = 7,
	Eight = 8,
	Nine = 9,
	Ten = 10,
	Jack = 11,
	Queen = 12,
	King = 13,
	Ace = 14
};

struct CardValue {
public:
	CardSuit suit;//la couleur
	CardRank rank;//la couleur
	CardValue(CardRank r, CardSuit s);
	std::string ToString();
	std::string ToShortString();
};