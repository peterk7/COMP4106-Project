#include "stdafx.h"

GameManager::GameManager() {
	log = Logger::getInstance();
	log->info(formatLog("Ctor"));

	model = shared_ptr<GameModel>(new GameModel());
	controller = shared_ptr<Controller>(new Controller(this));
	view = shared_ptr<View>(new View(this));
};

GameManager::~GameManager() {
	log->info(formatLog("Dtor"));
}

string GameManager::formatLog(string msg)
{
	return "{GameManager} " + msg;
}

void GameManager::startGame() {
	log->info(formatLog("startGame"));

	view->displayBoard();

	while (!model ->getIsGameEnd()) {
		// Get next input
		controller->preformNextPlay();

		if (constants::IS_STOP_BETWEEN_MOVES) {
			getchar();
		}

		// Update
		view->displayBoard();

		Sleep(constants::SECONDS_BETWEEN_MOVES * 1000);
	}

	string msg = "Winning player(s): " + model->getWinPlayers();
	log->info(formatLog(msg));

	log->info(formatLog("Game ended"));
}