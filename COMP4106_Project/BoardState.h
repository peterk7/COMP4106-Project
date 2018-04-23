#pragma once

#include "stdafx.h"

class BoardState {
public:
	BoardState();
	BoardState(const BoardState &other);
	~BoardState();

	bool operator<(const  BoardState & other) const;

	vector<int> playerHandSize;
	int deckSize;
	vector<Card> attack;
	vector<Card> defence;
	Card currentAttack;
	bool isAttacking;
	int currentPlayer;
	int prevPlayer;
	Card trumpCard;
	bool isWin;
	shared_ptr<Play> lastMove;

	vector<Card> getVisibleCards();
	vector<Play> getLegalPlays(vector<Card> * const playerHand, vector<Card> * const knownUnplayable);
	shared_ptr<BoardState> preformPlay(Play p);

	string toString(string baseIndent);

private:
	// Original - reduce by
	vector<Card> reduceCards(vector<Card> original, vector<Card> reduceBy);
	vector<Card> findCommon(vector<Card> a, vector<Card> b);
	int getCardPower(Card c);

	void nextPlayer();
	bool checkGameEnd();

	int getValue() const;
};