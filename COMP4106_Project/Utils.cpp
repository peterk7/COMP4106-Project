#include "stdafx.h"

namespace utils {
	string getTime(string format) {
		time_t rawtime;
		struct tm timeinfo;
		char buffer[80];

		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);

		strftime(buffer, sizeof(buffer), format.c_str(), &timeinfo);
		return string(buffer);
	}

	vector<string> split(const string& s, char delimiter)
	{
		vector<string> tokens;
		string token;
		istringstream tokenStream(s);
		while (getline(tokenStream, token, delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}

	string getRankStr(int r) {
		switch (r) {
		case constants::RANK_SIX:
			return constants::RANK_STR_SIX;
			break;
		case constants::RANK_SEVEN:
			return constants::RANK_STR_SEVEN;
			break;
		case constants::RANK_EIGHT:
			return constants::RANK_STR_EIGHT;
			break;
		case constants::RANK_NINE:
			return constants::RANK_STR_NINE;
			break;
		case constants::RANK_TEN:
			return constants::RANK_STR_TEN;
			break;
		case constants::RANK_JACK:
			return constants::RANK_STR_JACK;
			break;
		case constants::RANK_QUEEN:
			return constants::RANK_STR_QUEEN;
			break;
		case constants::RANK_KING:
			return constants::RANK_STR_KING;
			break;
		case constants::RANK_ACE:
			return constants::RANK_STR_ACE;
			break;
		}
		return constants::RANK_STR_HID;
	}

	string getSuitStr(int s) {
		switch (s) {
		case constants::SUIT_HEART:
			return constants::SUIT_STR_HEART;
			break;
		case constants::SUIT_DIAMOND:
			return constants::SUIT_STR_DIAMOND;
			break;
		case constants::SUIT_SPADES:
			return constants::SUIT_STR_SPADES;
			break;
		case constants::SUIT_CLUBS:
			return constants::SUIT_STR_CLUBS;
			break;
		}
		return constants::SUIT_STR_HID;
	}

	int getRankInt(string r) {
		if (r == constants::RANK_STR_SIX) {
			return constants::RANK_SIX;
		}
		else if (r == constants::RANK_STR_SEVEN) {
			return constants::RANK_SEVEN;
		}
		else if (r == constants::RANK_STR_EIGHT) {
			return constants::RANK_EIGHT;
		}
		else if (r == constants::RANK_STR_NINE) {
			return constants::RANK_NINE;
		}
		else if (r == constants::RANK_STR_TEN) {
			return constants::RANK_TEN;
		}
		else if (r == constants::RANK_STR_JACK) {
			return constants::RANK_JACK;
		}
		else if (r == constants::RANK_STR_QUEEN) {
			return constants::RANK_QUEEN;
		}
		else if (r == constants::RANK_STR_KING) {
			return constants::RANK_KING;
		}
		else if (r == constants::RANK_STR_ACE) {
			return constants::RANK_ACE;
		}
		else {
			return 0;
		}
	}

	int getSuitInt(string s) {
		if (s == constants::SUIT_STR_CLUBS || s == constants::SUIT_STR_CLUBS_SHORT) {
			return constants::SUIT_CLUBS;
		}
		else if (s == constants::SUIT_STR_SPADES || s == constants::SUIT_STR_SPADES_SHORT) {
			return constants::SUIT_SPADES;
		}
		else if (s == constants::SUIT_STR_DIAMOND || s == constants::SUIT_STR_DIAMOND_SHORT) {
			return constants::SUIT_DIAMOND;
		}
		else if (s == constants::SUIT_STR_HEART || s == constants::SUIT_STR_HEART_SHORT) {
			return constants::SUIT_HEART;
		}
		else {
			return 0;
		}
	}
}