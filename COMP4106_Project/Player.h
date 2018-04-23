#pragma once

#include "stdafx.h"
class Controller;

class Player : public Loggable {
public:
	Player(int id, Controller*, string type = "Player");
	~Player();

	string getType() { return type; }

	virtual shared_ptr<Play> getNextPlay(shared_ptr<BoardState> = NULL) = 0;

	string toString(string baseIndent = "");

protected:
	int id;
	string type;
	Controller* controller;
};