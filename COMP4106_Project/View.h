#pragma once

class GameManager;

class View : public Loggable {
public:
	View(GameManager * gm);
	~View();

	void displayBoard();

	string getUserInput(string msg);
	void displayMsg(string msg);

private:
	GameManager* manager;
	string lastMsg;
};