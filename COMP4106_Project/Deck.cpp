#include "stdafx.h"
#include "Deck.h"

Deck::Deck() : Loggable("Deck") {

	// Init container of cards
	vector<Card> cardList = getStartingDeck();

	// Shuffle cards
	random_shuffle(cardList.begin(), cardList.end());

	// Put into a queue
	for (auto it = cardList.begin(); it != cardList.end(); it++) {
		deck.push(*it);
	}

	// Pick the trump card
	trumpCard = deck.front();
	deck.pop();
	isEmpty = false;
}


Deck::~Deck() {

}

vector<Card> Deck::getStartingDeck() {
	vector<Card> cardList;

	for (int i = constants::SUIT_HEART; i <= constants::SUIT_SPADES; i++) {
		for (int j = constants::RANK_SIX; j <= constants::RANK_ACE; j++) {
			cardList.push_back(Card(j, i));
		}
	}
	return cardList;
}

Card Deck::drawCard() {
	if (isEmpty) {
		return Card();
	}

	if (deck.empty()) {
		isEmpty = true;
		return trumpCard;
	}

	Card tmp = deck.front();
	deck.pop();
	return tmp;
}

int Deck::getSize() {
	if (isEmpty) {
		return 0;
	}
	return deck.size() + 1;
}


string Deck::toString(string baseIndent) {
	ostringstream stringStream;
	stringStream << baseIndent << "Trump Card is: " + trumpCard.toString() + " . Deck has " + to_string(getSize()) + " cards.";
	return stringStream.str();
}
