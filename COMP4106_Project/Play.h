#pragma once

class Play {
public:
	Play(Card c, bool iea = false, bool ist = false);
	Play(const Play &other);
	~Play();

	bool operator==(const Play &other) const;
	bool operator<(const  Play & other) const;

	Card getCard() { return card; }
	bool getIsEndAttack() { return isEndAttack; }
	bool getIsTake() { return isTake; }

	string toString(string baseIndent = "");

private:
	Card card;
	bool isEndAttack;
	bool isTake;

	int getValue() const;
};