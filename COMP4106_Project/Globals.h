#pragma once

namespace constants
{
	const string TAB = "    ";

	enum RANKS { RANK_SIX = 6, RANK_SEVEN, RANK_EIGHT, RANK_NINE, RANK_TEN, RANK_JACK, RANK_QUEEN, RANK_KING, RANK_ACE };
	const string RANK_STR_HID = "";
	const string RANK_STR_TWO = "2";
	const string RANK_STR_THREE = "3";
	const string RANK_STR_FOUR = "4";
	const string RANK_STR_FIVE = "5";
	const string RANK_STR_SIX = "6";
	const string RANK_STR_SEVEN = "7";
	const string RANK_STR_EIGHT = "8";
	const string RANK_STR_NINE = "9";
	const string RANK_STR_TEN = "10";
	const string RANK_STR_JACK = "J";
	const string RANK_STR_QUEEN = "Q";
	const string RANK_STR_KING = "K";
	const string RANK_STR_ACE = "A";

	enum SUITS { SUIT_HEART = 1, SUIT_DIAMOND, SUIT_CLUBS, SUIT_SPADES };
	const string SUIT_STR_HID = "Hidden";
	const string SUIT_STR_HEART = "Heart";
	const string SUIT_STR_DIAMOND = "Diamond";
	const string SUIT_STR_CLUBS = "Clubs";
	const string SUIT_STR_SPADES = "Spades";
	const string SUIT_STR_HEART_SHORT = "H";
	const string SUIT_STR_DIAMOND_SHORT = "D";
	const string SUIT_STR_CLUBS_SHORT = "C";
	const string SUIT_STR_SPADES_SHORT = "S";

	const string PLAYER_HUMAN = "Human";
	const string PLAYER_RANDOM = "Random";
	const string PLAYER_AI_MONTE_CARLO = "MonteCarlo";

	// TODO: will not work if different from 2
	const int PLAYERS_NUMBER = 2;

	const int PLAYER_START_HAND_SIZE = 6;
	const string FINISH_ATTACK_STRING = "end";
	const string TAKE_CARDS_STRING = "take";

	const int NUMER_OF_SIMULATIONS = 500;
	const float STATE_NEGLECT_BOOST = 1.4;

	const float SECONDS_BETWEEN_MOVES = 0;
	const bool IS_STOP_BETWEEN_MOVES = false;
	const int LOGGER_LAST_LOGS_COUNT = 15;
	const bool IS_SHOW_PLAYERS = true;
	
} // namespace constants

namespace settings {
	extern int playerCount;
	extern string playerType1;
	extern string playerType2;
}