#pragma once

class BoardState;
class Player;

class PlayerRandom : public Player {
public:
	PlayerRandom(int id, Controller*);
	~PlayerRandom();

	shared_ptr<Play> getNextPlay(shared_ptr<BoardState> = NULL);

protected:
	set<Card> seenCards;
	vector<Card> hand;
};