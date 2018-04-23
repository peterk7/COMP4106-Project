#include "stdafx.h"
#include "PlayerRandom.h"


PlayerRandom::PlayerRandom(int id, Controller* c) : Player(id, c, constants::PLAYER_RANDOM) {};


PlayerRandom::~PlayerRandom() {
	log->info(formatLog("Dtor"));
}

shared_ptr<Play> PlayerRandom::getNextPlay(shared_ptr<BoardState> state) {

	log->info(formatLog("getNextPlay"));

	// Get current hand
	hand = controller->getModel()->getPlayerHand();
	for (auto it = hand.begin(); it != hand.end(); it++) {
		seenCards.insert(*it);
	}

	// Get visible cards in the state
	vector<Card> visible = state->getVisibleCards();
	// Update seen cards 
	for (auto it = visible.begin(); it != visible.end(); it++) {
		seenCards.insert(*it);
	}

	vector<Card> known(seenCards.begin(), seenCards.end());
	vector<Play> legalMoves = state->getLegalPlays(&hand, &known);

	int randomChoise = rand() % (legalMoves.size());

	return shared_ptr<Play>(new Play(legalMoves[randomChoise]));
}
