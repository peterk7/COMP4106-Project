#include "stdafx.h"

View::View(GameManager * gm) : Loggable("View") {
	log = Logger::getInstance();
	log->info(formatLog("Ctor"));

	manager = gm;
};

View::~View() {
	log->info(formatLog("Dtor"));
}

void View::displayBoard() {
	system("cls");
	string toDisplay = "";

	toDisplay += log->getLastLogs() + "\n";

	toDisplay += manager->getModel()->toString(constants::TAB) + "\n";

	cout << toDisplay << endl;

	if (lastMsg != "") {
		cout << lastMsg << endl << endl;
		lastMsg = "";
	}
}

string View::getUserInput(string msg) {
	log->info(formatLog("Get user input"));

	string ret;

	cout << endl << msg << endl;
	getline(cin, ret);
	return ret;
}

void View::displayMsg(string msg) {
	lastMsg = msg;
}
