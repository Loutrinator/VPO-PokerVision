#include "CardValue.h"

CardValue::CardValue(CardRank r, CardSuit s) : rank(r), suit(s) {}

std::string CardValue::ToString()
{
	std::string name;
	switch (rank) {
	case Two: name += "Two"; break;
	case Three: name += "Three"; break;
	case Four: name += "Four"; break;
	case Five: name += "Five"; break;
	case Six: name += "Six"; break;
	case Seven: name += "Seven"; break;
	case Eight: name += "Eight"; break;
	case Nine: name += "Nine"; break;
	case Ten: name += "Ten"; break;
	case Jack: name += "Jack"; break;
	case Queen: name += "Queen"; break;
	case King: name += "King"; break;
	case Ace: name += "Ace"; break;
	}
	name += " of ";
	switch (suit) {
	case Clubs: name += "Clubs"; break;
	case Spades: name += "Spades"; break;
	case Diamonds: name += "Diamonds"; break;
	case Hearts: name += "Hearts"; break;
	}
	return name;
}

std::ostream& operator<<(std::ostream& lhs, CardValue t) {
	return lhs << t.ToString();
}