#pragma once

class Card {
public:
	Card(int r = 0, int s = 0);
	Card(const Card &other);
	~Card();

	Card& operator=(const Card &other);
	bool operator==(const Card &other) const;

	bool operator<(const  Card & other) const;

	int getRank() const { return rank; };
	int getSuit() const { return suit; };

	string toString(string baseIndent = "");

private:
	int rank;
	int suit;
};