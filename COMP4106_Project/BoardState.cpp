#include "stdafx.h"

BoardState::BoardState()
{
	// Init with starting board

	deckSize = 0;
	isAttacking = false;
	currentPlayer = 0;
	isWin = false;
	lastMove = NULL;
	prevPlayer = 1;
};

BoardState::BoardState(const BoardState &other)
{
	// Copy the board state
	playerHandSize = other.playerHandSize;
	deckSize = other.deckSize;
	attack = other.attack;
	defence = other.defence;
	currentAttack = other.currentAttack;
	isAttacking = other.isAttacking;
	currentPlayer = other.currentPlayer;
	trumpCard = other.trumpCard;
	isWin = other.isWin;
	lastMove = other.lastMove;
	prevPlayer = other.prevPlayer;
}

BoardState::~BoardState()
{

}

bool BoardState::operator<(const  BoardState & other) const {
	return (getValue() < other.getValue());
}

vector<Card> BoardState::getVisibleCards() {
	vector<Card> visible;

	visible = attack;
	for (auto it = defence.begin(); it != defence.end(); it++) {
		visible.push_back(*it);
	}
	visible.push_back(currentAttack);

	return visible;
}

vector<Play> BoardState::getLegalPlays(vector<Card> * const playerHand, vector<Card> * const knownUnplayable) {
	vector<Play> legal;
	vector<Card> playableCards;

	if (isAttacking) {
		// Attack

		// Play card
		if (playerHandSize[0] > 0 && playerHandSize[1] > 0) {
		
			if (attack.size() != 0) {
				// History of attack exists

				// Get ranks of attack history
				set<int> ranks;
				for (auto it = attack.begin(); it != attack.end(); it++) {
					ranks.insert(it->getRank());
				}
				for (auto it = defence.begin(); it != defence.end(); it++) {
					ranks.insert(it->getRank());
				}

				// For each rank, create card in each suit
				vector<Card> possibleCards;
				for (auto it = ranks.begin(); it != ranks.end(); it++) {
					for (int j = constants::SUIT_HEART; j < constants::SUIT_SPADES; j++) {
						possibleCards.push_back(Card(*it, j));
					}
				}

				if (playerHand != NULL) {
					// Only cards from player hand
					playableCards = findCommon(*playerHand, possibleCards);
				}
				else {
					// Only playable cards
					playableCards = reduceCards(possibleCards, *knownUnplayable);
				}
			}
			else {
				// No history, can attack with any card

				if (playerHand != NULL) {
					playableCards = *playerHand;
				}
				else {
					playableCards = reduceCards(Deck::getStartingDeck(), *knownUnplayable);
				}
			}
		}

		// End attack
		if (attack.size() != 0) {
			legal.push_back(Play(Card(), true));
		}

	}
	else {
		// Defence

		// Play card
		int currentAttackPower = getCardPower(currentAttack);

		if (playerHandSize[0] > 0 && playerHandSize[1] > 0) {
			if (playerHand != NULL) {
				for (auto it = playerHand->begin(); it != playerHand->end(); it++) {
					if (getCardPower(*it) > currentAttackPower && (it->getSuit() == currentAttack.getSuit() || it->getSuit() == trumpCard.getSuit())) {
						playableCards.push_back(*it);
					}
				}
			}
			else {
				vector<Card> unknownCards = reduceCards(Deck::getStartingDeck(), *knownUnplayable);
				for (auto it = unknownCards.begin(); it != unknownCards.end(); it++) {
					if (getCardPower(*it) > currentAttackPower) {
						playableCards.push_back(*it);
					}
				}
			}
		}

		// Take
		legal.push_back(Play(Card(), false, true));
	}

	// Generate play for each card
	for (auto it = playableCards.begin(); it != playableCards.end(); it++) {
		legal.push_back(Play(*it));
	}

	return legal;
}

shared_ptr<BoardState> BoardState::preformPlay(Play p) {
	shared_ptr<BoardState> newState = shared_ptr<BoardState>(new BoardState(*this));

	if (newState->isAttacking) {
		if (p.getIsEndAttack()) {
			newState->currentAttack = Card();
			newState->attack.clear();
			newState->defence.clear();
			newState->isAttacking = true;

			while (newState->deckSize > 0 && newState->playerHandSize[0] < 6) {
				newState->deckSize--;
				newState->playerHandSize[0]++;
			}
			while (newState->deckSize > 0 && newState->playerHandSize[1] < 6) {
				newState->deckSize--;
				newState->playerHandSize[1]++;
			}

			newState->nextPlayer();
		}
		else {
			newState->currentAttack = p.getCard();
			newState->playerHandSize[currentPlayer]--;
			if (newState->playerHandSize[currentPlayer] < 0) {
				newState->playerHandSize[currentPlayer];
			}

			newState->isAttacking = false;
			newState->nextPlayer();
		}
	}
	else {
		if (p.getIsTake()) {
			int cardsToTake = 0;
			if (newState->currentAttack.getRank() != 0) {
				cardsToTake++;
			}
			cardsToTake += attack.size() + defence.size();
			newState->playerHandSize[currentPlayer] += cardsToTake;

			while (newState->deckSize > 0 && newState->playerHandSize[0] < 6) {
				newState->deckSize--;
				newState->playerHandSize[0]++;
			}
			while (newState->deckSize > 0 && newState->playerHandSize[1] < 6) {
				newState->deckSize--;
				newState->playerHandSize[1]++;
			}

			newState->currentAttack = Card();
			newState->attack.clear();
			newState->defence.clear();
			newState->isAttacking = true;
			newState->nextPlayer();
		}
		else {
			newState->attack.push_back(currentAttack);
			newState->defence.push_back(p.getCard());
			newState->currentAttack = Card();
			newState->playerHandSize[currentPlayer]--;
			if (newState->playerHandSize[currentPlayer] < 0) {
				newState->playerHandSize[currentPlayer];
			}


			newState->isAttacking = true;
			newState->nextPlayer();
		}
	}

	newState->lastMove = shared_ptr<Play>(new Play(p));

	return newState;
}

vector<Card> BoardState::reduceCards(vector<Card> original, vector<Card> reduceBy) {
	for (auto it = reduceBy.begin(); it != reduceBy.end(); it++) {
		original.erase(remove(original.begin(), original.end(), *it), original.end());
	}
	return original;
}

vector<Card> BoardState::findCommon(vector<Card> a, vector<Card> b) {
	vector<Card> common;
	for (auto it = a.begin(); it != a.end(); it++) {
		auto found = find(b.begin(), b.end(), *it);
		if (found != b.end()) {
			common.push_back(*it);
		}
	}
	return common;
}

int BoardState::getCardPower(Card c) {
	int power = c.getRank();
	if (c.getSuit() == trumpCard.getSuit()) {
		// Same suit, increase power
		power += constants::RANK_ACE;
	}
	return power;
}

void BoardState::nextPlayer() {
	checkGameEnd();
	if (!isWin) {
		prevPlayer = currentPlayer;
		currentPlayer = (currentPlayer + 1) % 2;
	}
}

bool BoardState::checkGameEnd() {
	if (deckSize != 0) {
		return false;
	}

	if (playerHandSize[0] == 0 || playerHandSize[1] == 0) {
		isWin = true;
		return true;
	}

	return false;
}

string BoardState::toString(string baseIndent) {
	string output = "";

	return output;
}

int BoardState::getValue() const {
	int val = 0;

	for (auto it = playerHandSize.begin(); it != playerHandSize.end(); it++) {
		val += *it;
	}

	for (auto it = attack.begin(); it != attack.end(); it++) {
		val += (it->getRank() + it->getSuit() * 100) * 1000;
	}
	for (auto it = defence.begin(); it != defence.end(); it++) {
		val += (it->getRank() + it->getSuit() * 100) * 1000;
	}
	val += (currentAttack.getRank() + currentAttack.getSuit() * 100) * 1000;
	val += (trumpCard.getRank() + trumpCard.getSuit() * 100) * 10000;

	val += currentPlayer * 20000;
	if (isAttacking) {
		val += 100000;
	}
	if (isWin) {
		val += 1000000;
	}

	return val;
}
