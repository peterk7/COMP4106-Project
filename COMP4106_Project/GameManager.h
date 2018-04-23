#pragma once

#include "stdafx.h"

class GameManager {
public:
	GameManager();
	~GameManager();

	void startGame();

	shared_ptr<GameModel> getModel() { return model; };
	shared_ptr<Controller> getController() { return controller; };
	shared_ptr<View> getView() { return view; };

private:
	Logger * log;

	shared_ptr<GameModel> model;
	shared_ptr<Controller> controller;
	shared_ptr<View> view;

	string formatLog(string msg);
};