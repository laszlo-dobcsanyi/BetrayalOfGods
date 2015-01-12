#ifndef LIST_PROCESSABLE_HPP
#define LIST_PROCESSABLE_HPP

#include <boost\foreach.hpp>

#include "ListLockable.hpp"

template<class T> class ListProcessable
{
public:
	ListLockable<T> data;
	ListLockable<T> adding;
	ListLockable<T> removing;

	void Add(T& _element) { adding.Add(_element); }
	void Remove(T& _element) { removing.Add(_element); }

	void Process_Adding()
	{
		if (0 < adding.number)
		{
			boost::unique_lock<boost::shared_mutex> adding_lock(adding.mutex);
			boost::unique_lock<boost::shared_mutex> data_lock(data.mutex);

			BOOST_FOREACH(T _elem, adding.list)
			{
				data.list.push_back(_elem);
			}
			data.number += adding.number;

			adding.list.clear();
			adding.number = 0;
		}
	}

	void Process_Adding(void (*_function)(T& _current))
	{
		if (0 < adding.number)
		{
			boost::unique_lock<boost::shared_mutex> adding_lock(adding.mutex);
			boost::unique_lock<boost::shared_mutex> data_lock(data.mutex);

			BOOST_FOREACH(T _elem, adding.list)
			{
				data.list.push_back(_elem);

				_function(_elem);
			}
			data.number += adding.number;

			adding.list.clear();
			adding.number = 0;
		}
	}

	void Process_Removing()
	{
		if (0 < removing.number)
		{
			boost::unique_lock<boost::shared_mutex> removing_lock(removing.mutex);
			boost::unique_lock<boost::shared_mutex> data_lock(data.mutex);

			BOOST_FOREACH(T _elem, removing.list)
			{
				data.list.remove(_elem);
			}
			data.number -= removing.number;

			removing.list.clear();
			removing.number = 0;
		}	
	}

	void Process_Removing(void (*_function)(T& _current))
	{
		if (0 < removing.number)
		{
			boost::unique_lock<boost::shared_mutex> removing_lock(removing.mutex);
			boost::unique_lock<boost::shared_mutex> data_lock(data.mutex);

			BOOST_FOREACH(T _elem, removing.list)
			{
				data.list.remove(_elem);

				_function(_elem);
			}
			data.number -= removing.number;

			removing.list.clear();
			removing.number = 0;
		}	
	}
};

#endif