#pragma once
class Loggable;

class Deck : public Loggable {
public:
	Deck();
	~Deck();

	static vector<Card> getStartingDeck();

	Card drawCard();
	Card getTrumpCard() { return trumpCard; }
	bool getIsEmpty() { return isEmpty; }
	int getSize();

	string toString(string baseIndent = "");

private:

	queue<Card> deck;
	Card trumpCard;
	bool isEmpty;
};

