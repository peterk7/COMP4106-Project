#pragma once

class BoardState;
class Player;

class PlayerHuman : public Player {
public:
	PlayerHuman(int id, Controller*);
	~PlayerHuman();

	shared_ptr<Play> getNextPlay(shared_ptr<BoardState> = NULL);

protected:
};