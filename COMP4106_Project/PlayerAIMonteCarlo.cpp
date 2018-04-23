#include "stdafx.h"

PlayerAIMonteCarlo::PlayerAIMonteCarlo(int id, Controller* c) : Player(id, c, constants::PLAYER_AI_MONTE_CARLO) {
	lastState = NULL;
};

PlayerAIMonteCarlo::~PlayerAIMonteCarlo() {
	log->info(formatLog("Dtor"));
}

shared_ptr<Play> PlayerAIMonteCarlo::getNextPlay(shared_ptr<BoardState> state) {

	log->info(formatLog("getNextPlay"));

	if (lastState != NULL && state->lastMove != NULL && state->lastMove->getIsTake()) {
		// If taken, remove cards from unplayable cards
		auto it = seenCards.find(lastState->currentAttack);
		if (it != seenCards.end()) {
			seenCards.erase(it);
		}
		for (auto it = lastState->attack.begin(); it != lastState->attack.end(); it++) {
			auto found = seenCards.find(*it);
			if (found != seenCards.end()) {
				seenCards.erase(found);
			}
		}
		for (auto it = lastState->defence.begin(); it != lastState->defence.end(); it++) {
			auto found = seenCards.find(*it);
			if (found != seenCards.end()) {
				seenCards.erase(found);
			}
		}
	}

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

	// Calculate what next move to take
	vector<Card> known(seenCards.begin(), seenCards.end());
	vector<Play> legalMoves = state->getLegalPlays(&hand, &known);

	if (legalMoves.size() <= 1) {
		// Only one move to make
		return shared_ptr<Play>(new Play(legalMoves[0]));
	}

	int games = 0;
	while (games < constants::NUMER_OF_SIMULATIONS) {
		run_simulation(shared_ptr<BoardState>(new BoardState(*state)));
		games++;
	}

	log->info(formatLog("Simulated " + to_string(games) + " games."));

	// Generate all possible states for current legal plays
	vector<shared_ptr<BoardState>> possibleMoves;
	for (auto it = legalMoves.begin(); it != legalMoves.end(); it++) {
		possibleMoves.push_back(state->preformPlay(*it));
	}

	shared_ptr<BoardState> best = NULL;
	float percentBest = 0;
	for (auto it = possibleMoves.begin(); it != possibleMoves.end(); it++) {
		int winings = wins[**it];
		int playings = plays[**it];
		float percent = (float)winings / playings;
		if (percent > percentBest) {
			best = *it;
			percentBest = percent;
		}
	}

	log->info(formatLog("Best state win percent: " + to_string(percentBest*100) + "%."));

	if (best == NULL) {
		log->error(formatLog("No best move found!"));
		return NULL;
	}

	lastState = state;

	return best->lastMove;
}

bool PlayerAIMonteCarlo::run_simulation(shared_ptr<BoardState> state) {
	set<Card> simulationKnownCards = seenCards;
	vector<Card> playerHand = hand;
	set<BoardState> visited;

	shared_ptr<BoardState> prev = NULL;
	bool isTake = false;

	bool isExpand = true;
	while (!(state->isWin)) {
		int player = state->currentPlayer;
		
		if (prev != NULL && state->lastMove != NULL && state->lastMove->getIsTake()) {
			// If taken, remove cards from unplayable cards
			auto found = simulationKnownCards.find(prev->currentAttack);
			if (found != simulationKnownCards.end()) {
				simulationKnownCards.erase(found);
			}
			for (auto it = prev->attack.begin(); it != prev->attack.end(); it++) {
				auto found = simulationKnownCards.find(*it);
				if (found != simulationKnownCards.end()) {
					simulationKnownCards.erase(found);
				}
			}
			for (auto it = prev->defence.begin(); it != prev->defence.end(); it++) {
				auto found = simulationKnownCards.find(*it);
				if (found != simulationKnownCards.end()) {
					simulationKnownCards.erase(found);
				}
			}
		}

		// Update known cards
		vector<Card> visible = state->getVisibleCards();
		// Update seen cards 
		for (auto it = visible.begin(); it != visible.end(); it++) {
			simulationKnownCards.insert(*it);
		}

		// Update player hand if current player
		if (player == id && playerHand.size() < state->playerHandSize[id]) {
			// Draw random cards
			bool canDraw = true;
			while (canDraw && playerHand.size() < state->playerHandSize[id]) {
				canDraw = drawRandom(&playerHand, simulationKnownCards);
			}
		}
		// Update seen cards from new hand cards
		for (auto it = hand.begin(); it != hand.end(); it++) {
			simulationKnownCards.insert(*it);
		}

		vector<Card> known(seenCards.begin(), seenCards.end());
		vector<Play> legalMoves;
		if (player == id) {
			legalMoves = state->getLegalPlays(&playerHand, &known);
		}
		else {
			legalMoves = state->getLegalPlays(NULL, &known);
		}

		// Generate state for each legal move
		vector<shared_ptr<BoardState>> stateVector;
		for (auto it = legalMoves.begin(); it != legalMoves.end(); it++) {
			stateVector.push_back(state->preformPlay(*it));
		}

		if (stateVector.size() == 1) {
			prev = state;
			state = stateVector[0];
		}
		else {
			// If all states have record of simulations
			bool isMissingRecord = false;
			for (auto it = stateVector.begin(); it != stateVector.end(); it++) {
				if (plays.find(**it) == plays.end()) {
					isMissingRecord = true;
					break;
				}
			}

			if (isMissingRecord) {
				// Not all states were simulated
				int randomChoise = rand() % (stateVector.size());
				prev = state;
				state = stateVector[randomChoise];
			}
			else {
				// Count total plays of all states
				int total = 0;
				for (auto it = stateVector.begin(); it != stateVector.end(); it++) {
					total += plays[**it];
				}

				shared_ptr<BoardState> bestState = NULL;
				float maxVal = 0;
				// Get best state out of all plays
				for (auto it = stateVector.begin(); it != stateVector.end(); it++) {
					float percent = (float)wins[**it] / plays[**it];
					// Neglect boost
					float boost = constants::STATE_NEGLECT_BOOST * sqrt(((float)total / plays[**it]));
					if (percent + boost > maxVal) {
						maxVal = percent + boost;
						bestState = shared_ptr<BoardState>(new BoardState(**it));
					}
				}

				prev = state;
				state = bestState;
			}
		}

		if (isExpand && plays.find(*state) == plays.end()) {
			// Add only first new state to list
			isExpand = false;
			plays[*state] = 0;
			wins[*state] = 0;
		}
		
		// Save all visited states
		visited.insert(*state);
	}

	for (auto it = visited.begin(); it != visited.end(); it++) {
		if (plays.find(*it) == plays.end()) {
			continue;
		}
		plays[*it] += 1;
		if (it->currentPlayer != state->currentPlayer) {
			// Same as winner
			wins[*it] += 1;
		}
	}

	if (state->currentPlayer == id) {
		return true;
	}

	return false;
}

bool PlayerAIMonteCarlo::drawRandom(vector<Card> *playerHand, set<Card> knownCards) {
	vector<Card> unknownDeck = Deck::getStartingDeck();
	for (auto it = knownCards.begin(); it != knownCards.end(); it++) {
		unknownDeck.erase(remove(unknownDeck.begin(), unknownDeck.end(), *it), unknownDeck.end());
	}

	if (unknownDeck.size() == 0) {
		return false;
	}
	int randomChoise = rand() % (unknownDeck.size());
	playerHand->push_back(unknownDeck[randomChoise]);
	return true;
}

