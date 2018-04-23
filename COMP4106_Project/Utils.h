#pragma once

namespace utils
{
	string getTime(string format = "%d-%m-%Y %I:%M:%S");
	vector<string> split(const string& s, char delimiter);

	string getRankStr(int);
	string getSuitStr(int);
	int getRankInt(string);
	int getSuitInt(string);

} // namespace utils