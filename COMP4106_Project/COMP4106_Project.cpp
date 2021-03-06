// Assignment1.cpp : Defines the entry point for the console application.
// Argument parsing taken from http://www.cplusplus.com/forum/articles/13355/ and modified

#include "stdafx.h"

string help()
{
	return "Usage is [-p1 <Random/Human/MonteCarlo>] [-p2 <Random/Human/MonteCarlo>]";
}

int main(int argc, char *argv[])
{
	srand(time(0));

	Logger* log = Logger::getInstance();

	string player1 = settings::playerType1;
	string player2 = settings::playerType2;

	log->info("Start Main");

	for (int i = 1; i < argc; i++) { /* We will iterate over argv[] to get the parameters stored inside.
									 * Note that we're starting on 1 because we don't need to know the
									 * path of the program, which is stored in argv[0] */
		if (string(argv[i]) == "-p1" && i + 1 < argc) {
			i++;
			settings::playerType1 = string(argv[i]);
		}
		else if (string(argv[i]) == "-p2" && i + 1 < argc) {
			i++;
			settings::playerType2 = string(argv[i]);
		}
		else if (string(argv[i]) == "-help") {
			log->info(help());
			exit(0);
		}
		else {
			log->info(help());
			log->info("Not enough or invalid arguments, please try again.\n");
			exit(0);
		}
	}

	unique_ptr<GameManager> g(new GameManager());

	g->startGame();

	log->info("End Main");
	return 0;
}

