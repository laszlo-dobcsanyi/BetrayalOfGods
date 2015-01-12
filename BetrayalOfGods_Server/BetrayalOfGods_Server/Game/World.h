#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <list>
#include <string>

#include <boost\thread.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\enable_shared_from_this.hpp>

#include "Core\ListProcessable.hpp"
#include "Game\Region.h"

class server;
class Field;
class Field_Loaded;
class Field_Generated;
class Character;
class Group;

struct Field_Descriptor
{
	Field_Loaded* field_loaded;
	
	int TileID;

	Field_Descriptor()
	{
		field_loaded = 0;
		TileID = 0;
	}

	void Loaded(Field_Loaded* _field_loaded)
	{
		field_loaded = _field_loaded;
	}

	void Generated(int& _tileid)
	{
		TileID = _tileid;
	}
};


struct Area
{
	std::list<Field_Loaded*> fields;
	boost::shared_ptr<boost::thread> thread;

	void Run() { thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Area::Update, this))); }
	void Update();
};


class World
	: public boost::enable_shared_from_this<World>
{
public:
	int id;
	
	ListProcessable<boost::shared_ptr<Group> > groups;

	World(const int& _id);

	inline const Field_Loaded* Loaded(const Region& _region) const { return (fields[_region.x + _region.y * width].field_loaded); };
	Field* GetField(const int& _x, const int& _y);

private:
	int width;
	int height;

	Area* areas;
	Field_Descriptor* fields;
};
#endif