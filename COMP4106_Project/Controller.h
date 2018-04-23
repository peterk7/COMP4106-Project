#pragma once

class GameManager;
class GameModel;
class Player;

class Controller : public Loggable {
public:
	Controller(GameManager * gm);
	~Controller();

	shared_ptr<GameModel> getModel();

	void preformNextPlay();

	string waitForInput(string msg);

	string toString(string baseIndent = "");

private:

	static const string humanType;

	GameManager* manager;

	Player** players;

	Player* createPlayer(int id, string type);
};