#ifndef GAME_GROUP_H
#define GAME_GROUP_H

#define GROUP_UPDATE_INTERVAL 5

#include <thread>
#include <boost\thread.hpp>
#include <boost\shared_ptr.hpp>

#include "Core\ListProcessable.hpp"

class World;
class Region;
class Character;

class Group
	: public ListProcessable_Callback< boost::shared_ptr<Character> >
{
public:
	ListProcessable<boost::shared_ptr<Character> > characters;

	Group(boost::shared_ptr<Character> _leader);
	~Group();

private:
	bool disposed;

	boost::shared_ptr<Character> leader;

	void Update();
	boost::shared_ptr<boost::thread> thread;

	void Update_Center(Region const * _old, const Region const * _new);
	Region const * Move_Center(Region const * _center, const int& _direction);

	void ProcessAdding(boost::shared_ptr<Character> _character);
	void ProcessRemoving(boost::shared_ptr<Character> _character);

	void Dispose();
};

#endif