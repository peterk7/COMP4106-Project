#pragma once

class BoardState;
class Player;

class PlayerAIMonteCarlo : public Player {
public:
	PlayerAIMonteCarlo(int id, Controller*);
	~PlayerAIMonteCarlo();

	shared_ptr<Play> getNextPlay(shared_ptr<BoardState> = NULL);

protected:
	set<Card> seenCards;
	vector<Card> hand;
	shared_ptr<BoardState> lastState;

	bool run_simulation(shared_ptr<BoardState> state);
	bool drawRandom(vector<Card> *playerHand, set<Card> knownCards);

	map<BoardState, int> plays;
	map<BoardState, int> wins;
};