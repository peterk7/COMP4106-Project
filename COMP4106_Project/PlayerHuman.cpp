#include "stdafx.h"

PlayerHuman::PlayerHuman(int id, Controller* c) : Player(id, c, constants::PLAYER_HUMAN) {
	
};

PlayerHuman::~PlayerHuman() {
	log->info(formatLog("Dtor"));
}

shared_ptr<Play> PlayerHuman::getNextPlay(shared_ptr<BoardState>) {
	// Get input from player
	shared_ptr<Play> ret = NULL;
	bool isValid = false;

	string errorPars = "Couldnt parse your input, please try again.\n";
	string msg = "Please enter card to play:\n";
	msg += "\"R T\" where R is in {6,7,8,9,10,J,Q,K,A} and T is in {H,D,S,C}.\n";
	msg += "\"" + constants::FINISH_ATTACK_STRING + "\" to finish the attack.\n";
	msg += "\"" + constants::TAKE_CARDS_STRING + "\" to take cards.\n";
	string currMsg = msg;

	while (!isValid) {
		string userInput = controller->waitForInput(currMsg);
		log->info(formatLog("Recieved: \"" + userInput + "\""));

		currMsg = errorPars + msg;

		// Parse input
		vector<string> split = utils::split(userInput, ' ');

		if (split.size() == 1) {
			if (split[0] == constants::FINISH_ATTACK_STRING) {
				ret = shared_ptr<Play>(new Play(Card(), true));
				isValid = true;
				continue;
			}
			else if (split[0] == constants::TAKE_CARDS_STRING) {
				ret = shared_ptr<Play>(new Play(Card(), false, true));
				isValid = true;
				continue;
			}
		}

		if (split.size() != 2) {
			// No two inputs recieved
			continue;
		}

		int rank = utils::getRankInt(split[0]);
		int suit = utils::getSuitInt(split[1]);
		if (rank == 0 || suit == 0) {
			// Inputs are not what we expected
			continue;
		}

		// Create play
		ret = shared_ptr<Play>(new Play(Card(rank, suit), false));
		
		// Check valid play
		isValid = true;
	}

	return ret;
}
