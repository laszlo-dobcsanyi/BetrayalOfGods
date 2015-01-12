#ifndef CORE_GENERATOR_H
#define CORE_GENERATOR_H

#include <boost\thread.hpp>

class Generator
{
public:
	Generator(const int& _range);

	int Next();
	void Return(int _value);

	void Dispose();

private:
	bool disposed;

	int current;
	int* values;
	boost::shared_mutex mutex;
};

#endif