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

std::string CardValue::ToShortString()
{
	std::string name;
	switch (rank) {
	case Two: name += "2"; break;
	case Three: name += "3"; break;
	case Four: name += "4"; break;
	case Five: name += "5"; break;
	case Six: name += "6"; break;
	case Seven: name += "7"; break;
	case Eight: name += "8"; break;
	case Nine: name += "9"; break;
	case Ten: name += "10"; break;
	case Jack: name += "J"; break;
	case Queen: name += "Q"; break;
	case King: name += "K"; break;
	case Ace: name += "A"; break;
	}
	switch (suit) {
	case Clubs: name += "C"; break;
	case Spades: name += "S"; break;
	case Diamonds: name += "D"; break;
	case Hearts: name += "H"; break;
	}
	return name;
}


std::ostream& operator<<(std::ostream& lhs, CardValue t) {
	return lhs << t.ToString();
}