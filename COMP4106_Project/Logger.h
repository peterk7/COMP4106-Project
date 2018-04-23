#pragma once

class SingletonDestroyer;

class Logger {
public:
	static Logger* getInstance();
	void info(string str);
	void error(string str);
	~Logger();

	string getLastLogs();

private:
	friend class SingletonDestroyer;
	list<string> lastLogs;
	int lastLogsSize;

	string filePath;
	ofstream logFile;
	static Logger* instance;
	static SingletonDestroyer _destroyer;

	Logger();
	void record(string msg);
};

class SingletonDestroyer
{
public:
	SingletonDestroyer(Logger * = 0);
	~SingletonDestroyer();
	void SetSingleton(Logger *s);

private:
	Logger * _singleton;
};