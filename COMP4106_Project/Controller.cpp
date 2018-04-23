#include "stdafx.h"

const string Controller::humanType = constants::PLAYER_HUMAN;

Controller::Controller(GameManager* gm) : Loggable("Controller") {

	manager = gm;

	players = new Player*[2];
	players[0] = createPlayer(0, settings::playerType1);
	players[1] = createPlayer(1, settings::playerType2);

	if (players[0] == NULL || players[1] == NULL) {
		log->info(formatLog("Error creating player!"));
	}
};

Player* Controller::createPlayer(int id, string type) {
	if (type == humanType) {
		return new PlayerHuman(id, this);
	}
	else if (type == constants::PLAYER_AI_MONTE_CARLO) {
		manager->getModel()->setAIPlayer(id);
		return new PlayerAIMonteCarlo(id, this);
	}
	else if (type == constants::PLAYER_RANDOM) {
		manager->getModel()->setAIPlayer(id);
		return new PlayerRandom(id, this);
	}
	return NULL;
}

Controller::~Controller() {
	log->info(formatLog("Dtor"));

	delete players[0];
	delete players[1];
	delete[] players;
}

shared_ptr<GameModel> Controller::getModel() { 
	return manager->getModel();
}

void Controller::preformNextPlay() {
	log->info(formatLog("Preform Next Move"));
	Player *currentPlayer = players[manager->getModel()->getCurrentPlayer()];
	shared_ptr<BoardState> bs = manager->getModel()->getBoardState();

	shared_ptr<Play> play = currentPlayer->getNextPlay(bs);

	if (play == NULL) {
		manager->getView()->displayMsg("No move selected, please try again.");
		return;
	}

	// Preform play
	bool isValid = manager->getModel()->preformPlay(play);
	if (!isValid) {
		// Display wrong move msg
		manager->getView()->displayMsg("Move is not allowed, please try again.");
	}
}

string Controller::waitForInput(string msg) {
	// Get input from view
	return manager->getView()->getUserInput(msg);
}

string Controller::toString(string baseIndent) {
	string ret = "";
	return ret;
}
