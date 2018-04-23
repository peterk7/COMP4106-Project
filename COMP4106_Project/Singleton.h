#pragma once

template <class T>
class Singleton
{
public:
	~Singleton();

private:
	friend class SingletonDestroyer;
	static Singleton* instance;
	static SingletonDestroyer _destroyer;

	Singleton();
};

template <class T>
class SingletonDestroyer
{
public:
	SingletonDestroyer(Singleton * = 0);
	~SingletonDestroyer();
	void SetSingleton(Singleton *s);

private:
	Singleton * _singleton;
};
