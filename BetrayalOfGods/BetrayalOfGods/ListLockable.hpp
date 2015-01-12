#ifndef LIST_LOCKABLE_HPP
#define LIST_LOCKABLE_HPP

#include <list>
#include <boost\thread.hpp>

template<class T> class ListLockable
{
public:
	int number;
	std::list<T> list;
	boost::shared_mutex mutex;

	ListLockable() : number(0) { }

	void Add(T& _element)
	{
		boost::unique_lock<boost::shared_mutex> lock(mutex);
		list.push_back(_element);
		++number;
	}

	void Remove(T& _element)
	{
		boost::unique_lock<boost::shared_mutex> lock(mutex);
		list.remove(_element);
		--number;
	}

	void Clear(T& _element)
	{
		boost::unique_lock<boost::shared_mutex> lock(mutex);
		list.clear();
		number = 0;
	}
};

#endif