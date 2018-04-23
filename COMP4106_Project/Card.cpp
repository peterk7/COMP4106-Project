#include "stdafx.h"
#include "Card.h"


Card::Card(int r, int s) {
	rank = r;
	suit = s;
}

Card::Card(const Card &other) {
	rank = other.rank;
	suit = other.suit;
}


Card::~Card() {
	// None
}

Card& Card::operator=(const Card &other) {
	if (this == &other) {
		return *this;
	}

	rank = other.rank;
	suit = other.suit;
	return *this;
}

bool Card::operator==(const Card &other) const {
	return (rank == other.rank && suit == other.suit);
}

bool Card::operator<(const  Card & other) const {
	// a < b
	int powerA = suit + rank * 100;

	int powerB = other.suit + other.rank * 100;

	return powerA < powerB;
}

string Card::toString(string baseIndent) {
	ostringstream stringStream;
	stringStream << baseIndent << setw(2) << utils::getRankStr(rank) << setw(8) << utils::getSuitStr(suit);
	return stringStream.str();
}
