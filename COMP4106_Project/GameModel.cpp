#include "stdafx.h"

GameModel::GameModel() : Loggable("GameModel") {

	isGameEnd = false;

	// Init all data of the game

	// Create a full deck
	deck = unique_ptr<Deck>(new Deck());

	handSize = constants::PLAYER_START_HAND_SIZE;

	// Create players
	playerCount = settings::playerCount;
	players = new vector<Card>*[playerCount];
	for (int i = 0; i < playerCount; i++) {
		players[i] = new vector<Card>();
		for (int j = 0; j < handSize; j++) {
			// Give player a card
			Card c = deck->drawCard();
			if (c.getSuit() != 0) {
				players[i]->push_back(c);
			}
			else {
				log->info(formatLog("Deck is empty!"));
			}
		}
	}

	currentPlayer = 0;
	isAttacking = true;
	currentAttack = NULL;

	boardState = unique_ptr<BoardState>(new BoardState());
};

GameModel::~GameModel() {
	log->info(formatLog("Dtor"));

	for (int i = 0; i < playerCount; i++) {
		delete players[i];
	}
	delete[] players;
}

shared_ptr<BoardState> GameModel::getBoardState() {
	shared_ptr<BoardState> bs = shared_ptr<BoardState>(new BoardState(*boardState));

	for (int i = 0; i < playerCount; i++) {
		bs->playerHandSize.insert(bs->playerHandSize.begin()+i, players[i]->size());
	}
	bs->deckSize = deck->getSize();
	bs->attack = attack;
	bs->defence = defence;
	if (currentAttack != NULL) {
		bs->currentAttack = *currentAttack;
	}
	bs->isAttacking = isAttacking;
	bs->currentPlayer = currentPlayer;
	bs->trumpCard = deck->getTrumpCard();
	bs->isWin = isGameEnd;
	bs->lastMove = lastPlay;

	return bs;
}

void GameModel::setAIPlayer(int id) {
	aiPlayers.push_back(id);
}

void GameModel::nextPlayer() {
	currentPlayer = (currentPlayer + 1) % 2;
	checkGameEnd();
}

bool GameModel::preformPlay(shared_ptr<Play> play) {

	string msg = "Next play: " + play->toString() + " by player " + to_string(currentPlayer + 1);
	log->info(formatLog(msg));

	if (play->getIsEndAttack()) {
		// End attack
		if (!isAttacking) {
			return false;
		}

		attack.clear();
		defence.clear();

		// Draw card for each player
		drawCardsToPlayers();

		isAttacking = true;
		nextPlayer();
		lastPlay = play;
		return true;
	}

	if (play->getIsTake()) {
		// Take cards on defence
		if (isAttacking) {
			return false;
		}

		// Give attack and defence cards to current player
		vector<Card> *playerHand = players[currentPlayer];

		if (currentAttack != NULL) {
			playerHand->push_back(*currentAttack);
			currentAttack = NULL;
		}

		for (auto it = defence.begin(); it != defence.end(); it++) {
			playerHand->push_back(*it);
		}
		for (auto it = attack.begin(); it != attack.end(); it++) {
			playerHand->push_back(*it);
		}

		attack.clear();
		defence.clear();

		// Draw card for each player
		drawCardsToPlayers();

		isAttacking = true;
		nextPlayer();
		lastPlay = play;
		return true;
	}

	// Check if move valid
	bool isValid = isValidMove(play->getCard());

	if (!isValid) {
		return false;
	}

	// Preform the play
	if (isAttacking) {
		// Attacking

		// Remove card from player hand
		removeCardFromPlayer(currentPlayer, play->getCard());

		// Set card as current attack
		currentAttack = make_shared<Card>(play->getCard());
		isAttacking = false;
		nextPlayer();
		lastPlay = play;
		return true;
	}
	else {
		// Defending

		removeCardFromPlayer(currentPlayer, play->getCard());

		attack.push_back(*currentAttack);
		defence.push_back(play->getCard());
		currentAttack = NULL;

		isAttacking = true;
		nextPlayer();
		lastPlay = play;
		return true;
	}

	log->error(formatLog("preformPlay unknown"));
	return false;
}

bool GameModel::checkGameEnd() {
	if (!deck->getIsEmpty()) {
		return false;
	}

	winPlayers = "";
	int nonEmptyPlayers = 0;
	for (int i = 0; i < playerCount; i++) {
		if (players[i]->size() != 0) {
			nonEmptyPlayers++;
		}
		else {
			winPlayers += "Player " + to_string(i + 1) + ", ";
		}
	}

	if (nonEmptyPlayers > 1) {
		winPlayers = "";
		return false;
	}

	winPlayers.pop_back();
	winPlayers.pop_back();

	isGameEnd = true;
	log->info(formatLog("Game has ended!"));
	return true;
}

bool GameModel::isValidMove(Card card) {

	// Check if card in player hand
	vector<Card> *playerHand = players[currentPlayer];
	auto it = find(playerHand->begin(), playerHand->end(), card);;
	if (it == playerHand->end()) {
		log->info(formatLog("Card not in player hand."));
		return false;
	}

	if (isAttacking && attack.size() > 0) {
		// Check if can attack using this card
		
		// Check for same rank in attack pile
		for (auto it = attack.begin(); it != attack.end(); it++) {
			if (it->getRank() == card.getRank()) {
				return true;
			}
		}

		// Check for same rank in defence pile
		for (auto it = defence.begin(); it != defence.end(); it++) {
			if (it->getRank() == card.getRank()) {
				return true;
			}
		}

		log->info(formatLog("No match to attack or defence history"));
		return false;
	}
	else if (isAttacking) {
		return true;
	}
	else if (!isAttacking) {
		// Check if can defend using this card
		
		// Check suit match
		if (card.getSuit() != currentAttack->getSuit() && card.getSuit() != deck->getTrumpCard().getSuit()) {
			log->info(formatLog("No suit match"));
			return false;
		}


		// Calc defending card power
		int defPower = card.getRank();
		if (card.getSuit() == deck->getTrumpCard().getSuit()) {
			// Same suit, increase power
			defPower += constants::RANK_ACE;
		}

		// Calc attacking card power
		int attPower = currentAttack->getRank();
		if (currentAttack->getSuit() == deck->getTrumpCard().getSuit()) {
			// Same suit, increase power
			attPower += constants::RANK_ACE;
		}

		log->info(formatLog("Check if can defend: att = " + to_string(attPower) + " vs def = " + to_string(defPower)));
		if (defPower > attPower ) {
			return true;
		}

		return false;
	}

	log->error(formatLog("isValidMove unknown"));
	return false;
}

string GameModel::toString(string baseIndent) {
	ostringstream stringStream;
	string newLine = "\n" + baseIndent;

	stringStream << baseIndent << deck->toString(baseIndent) << newLine;
	if (isAttacking) {
		stringStream << "Player attacking." << newLine << newLine;
	}
	else {
		stringStream << "Player defending." << newLine << newLine;
	}

	// Enemy player
	for (int i = 0; i < playerCount; i++) {
		if (i != currentPlayer) {
			// Print player
			stringStream << "Player " << (i+1) << ": " << strPlayer(i) << newLine;
		}
	}
	stringStream << newLine;

	// Display attack history
	stringStream << "Attack history:" << newLine;

	stringStream << baseIndent << "Attack : ";
	for (auto it = attack.begin(); it != attack.end(); it++) {
		if (it != attack.begin()) {
			stringStream << ", ";
		}
		stringStream << it->toString();
	}
	stringStream << newLine;

	// Display defence history
	stringStream << baseIndent << "Defence: ";
	for (auto it = defence.begin(); it != defence.end(); it++) {
		if (it != defence.begin()) {
			stringStream << ", ";
		}
		stringStream << it->toString();
	}
	stringStream << newLine << newLine;

	// Display current attack
	if (currentAttack != NULL) {
		stringStream << "Current Attack: " << currentAttack->toString() << newLine << newLine;
	}


	// Current player
	bool hideCurrenPlayer = find(aiPlayers.begin(), aiPlayers.end(), currentPlayer) != aiPlayers.end();
	stringStream << "Player " << (currentPlayer + 1) << ": " << strPlayer(currentPlayer, hideCurrenPlayer);
	

	return stringStream.str();
}

void GameModel::removeCardFromPlayer(int playerIndex, Card c) {
	// Remove card from player hand
	vector<Card> *playerHand = players[playerIndex];
	int index = distance(playerHand->begin(), find(playerHand->begin(), playerHand->end(), c));
	playerHand->erase(playerHand->begin() + index);
}

void GameModel::drawCardsToPlayers() {
	int index = (currentPlayer + 1) % 2;
	for (int i = 0; i < playerCount; i++) {
		vector<Card> *playerHand = players[index];
		if (playerHand->size() < handSize) {
			for (int j = playerHand->size(); j < handSize; j++) {
				// Give player a card
				Card c = deck->drawCard();
				if (c.getSuit() != 0) {
					playerHand->push_back(c);
				}
				else {
					log->info(formatLog("Deck is empty!"));
				}
			}
		}

		index = (index + 1) % 2;
	}
}

string GameModel::strPlayer(int index, bool isHidden) {
	string ret = "";

	isHidden = constants::IS_SHOW_PLAYERS ? false : isHidden;

	if (!isHidden) {
		sort(players[index]->begin(), players[index]->end());
	}

	for (auto it = players[index]->begin(); it != players[index]->end(); it++) {
		if (it != players[index]->begin()) {
			ret += ", ";
		}
		if (isHidden) {
			// Print hidden cards
			ret += Card().toString();
		}
		else {
			ret += it->toString();
		}
	}

	return ret;
}
