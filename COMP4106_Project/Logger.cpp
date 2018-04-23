#include "stdafx.h"

#pragma warning(disable:4996)

Logger* Logger::instance = 0;
SingletonDestroyer Logger::_destroyer;

Logger::Logger() {

	// Generate filename
	string time = utils::getTime("%y%m%d_%I%M%S");
	
	string fileName = time + "_A2.log";

	string dirName = "Logs";
	CreateDirectoryA(dirName.c_str(), NULL);
	filePath = dirName + "\\" + fileName;

	logFile.open(filePath, ios::app | ios::out);

	lastLogsSize = constants::LOGGER_LAST_LOGS_COUNT;

	info("Logger Ctor");
};

Logger::~Logger() {
	info("Logger Dtor");
	logFile << "~~~~~~~~~~" << endl << endl;
	logFile.close();
}

SingletonDestroyer::SingletonDestroyer(Logger *s)
{
	_singleton = s;
}

SingletonDestroyer::~SingletonDestroyer()
{
	delete _singleton;
}

void SingletonDestroyer::SetSingleton(Logger *s)
{
	_singleton = s;
}

Logger* Logger::getInstance() {
	if (instance == 0) {
		instance = new Logger();
		_destroyer.SetSingleton(instance);
	}
	return instance;
};

string Logger::getLastLogs() {
	string ret = "";

	for (list<string>::iterator it = lastLogs.begin(); it != lastLogs.end(); ++it) {
		ret += *it + "\n";
	}

	return ret;
}

void Logger::info(string str) {

	string timeStr = utils::getTime("%Y-%m-%d %I:%M:%S");

	string logStr = timeStr + " [INFO] : " + str;

	record(logStr);
}

void Logger::error(string str) {
	string timeStr = utils::getTime("%Y-%m-%d %I:%M:%S");

	string logStr = timeStr + " [ERROR] : " + str;

	record(logStr);
}

void Logger::record(string msg) {
	cout << msg << endl;

	if (logFile.is_open()) {
		logFile << msg << endl;
	}

	// Record log in the last logs
	lastLogs.push_back(msg);
	if (lastLogs.size() > lastLogsSize) {
		lastLogs.pop_front();
	}
}
