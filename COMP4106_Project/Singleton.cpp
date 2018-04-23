#include "stdafx.h"
#include "Singleton.h"

template <class T>
Singleton* Singleton::instance = 0;
template <class T>
SingletonDestroyer Singleton::_destroyer;

template <class T>
Singleton::Singleton()
{
}

template <class T>
Singleton::~Singleton()
{
}

template <class T>
SingletonDestroyer::SingletonDestroyer(Singleton *s)
{
	_singleton = s;
}

template <class T>
SingletonDestroyer::~SingletonDestroyer()
{
	delete _singleton;
}

template <class T>
void SingletonDestroyer::SetSingleton(Singleton *s)
{
	_singleton = s;
}

template <class T>
Singleton* Singleton::getInstance() {
	if (instance == 0) {
		instance = new Logger();
		_destroyer.SetSingleton(instance);
	}
	return instance;
};
