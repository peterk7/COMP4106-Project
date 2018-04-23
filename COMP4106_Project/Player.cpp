#include "stdafx.h"

Player::Player(int id, Controller* cont, string t) : Loggable(t) {

	this->id = id;
	type = t;
	controller = cont;
};

Player::~Player() {
	log->info(formatLog("Dtor"));
}

string Player::toString(string baseIndent) {
	string ret = "";
	ret += type;
	return ret;
}