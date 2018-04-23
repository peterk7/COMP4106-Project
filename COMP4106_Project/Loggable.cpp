#include "stdafx.h"
#include "Loggable.h"


Loggable::Loggable(string cn) {
	className = cn;
	log = Logger::getInstance();
	log->info(formatLog("Ctor"));
}


Loggable::~Loggable() {
	// None
}

string Loggable::formatLog(string log) {
	return "{" + className + "} " + log;
}
