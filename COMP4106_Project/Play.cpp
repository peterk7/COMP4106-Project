#include "stdafx.h"

Play::Play(Card c, bool iea, bool ist) {
	card = c;
	isEndAttack = iea;
	isTake = ist;
};

Play::Play(const Play &other) {
	card = other.card;
	isEndAttack = other.isEndAttack;
	isTake = other.isTake;
}

Play::~Play() {
	// No need
}

bool Play::operator==(const Play &other) const {
	return card == other.card && isEndAttack == other.isEndAttack;
}

bool Play::operator<(const  Play & other) const {
	return (getValue() < other.getValue());
}

string Play::toString(string baseIndent) {
	string ret = "Play(";
	string endAttack = isEndAttack ? "true" : "false";
	string takeCards = isTake ? "true" : "false";
	ret += card.toString() + ", endAttack: " + endAttack + ", takeCards: " + takeCards;
	ret += ")";
	return ret;
}

int Play::getValue() const {
	int val = 0;

	val += card.getRank() + card.getSuit() * 100;
	if (isEndAttack) {
		val = 1000;
	}
	if (isTake) {
		val = 2000;
	}

	return val;
}
