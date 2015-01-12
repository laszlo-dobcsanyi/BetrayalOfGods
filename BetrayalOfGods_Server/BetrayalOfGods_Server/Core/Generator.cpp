#include "Generator.h"

Generator::Generator(const int& _range)
	: disposed(false),
	  current(_range)
{
	values = new int[_range];
	for(int i = 0; i < _range; i++)
		values[i] = i;
}

int Generator::Next()
{
	boost::unique_lock<boost::shared_mutex> lock(mutex);
	current--;
	return values[current];
}

void Generator::Return(int _value)
{
	boost::unique_lock<boost::shared_mutex> lock(mutex);
	values[current] = _value;
	current++;
}

void Generator::Dispose()
{
	if (disposed) return; disposed = true;
	delete[] values;
}
