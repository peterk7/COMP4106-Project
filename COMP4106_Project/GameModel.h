#pragma once

#include "stdafx.h"

class GameModel : public Loggable {
public:
	GameModel();
	~GameModel();

	shared_ptr<BoardState> getBoardState();
	bool getIsGameEnd() { return isGameEnd; }
	int getCurrentPlayer() { return currentPlayer; }

	void setAIPlayer(int id);

	vector<Card> getPlayerHand() { return *players[currentPlayer]; };

	void nextPlayer();

	bool preformPlay(shared_ptr<Play>);
	bool checkGameEnd();
	string getWinPlayers() { return winPlayers; };

	string toString(string baseIndent = "");

private:

	unique_ptr<BoardState> boardState;

	bool isGameEnd;
	string winPlayers;
	int handSize;
	unique_ptr<Deck> deck;
	int playerCount;
	vector<Card> **players;
	int currentPlayer;
	vector<int> aiPlayers;
	shared_ptr<Play> lastPlay;

	shared_ptr<Card> currentAttack;
	vector<Card> attack;
	vector<Card> defence;
	bool isAttacking;

	bool isValidMove(Card card);
	void removeCardFromPlayer(int playerIndex, Card c);
	void drawCardsToPlayers();

	string strPlayer(int index, bool isHidden = true);
};