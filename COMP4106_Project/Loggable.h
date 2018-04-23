#pragma once
class Loggable
{
public:
	Loggable(string className = "Loggable");
	~Loggable();

protected:
	Logger * log;
	string className;

	string formatLog(string log);
};

