#include "Core\Macro.h"
#include "Game\Group.h"

#include <vector>

#include <boost\foreach.hpp>

#include "Core\Server.h"
#include "Core\Message.hpp"

#include "Manager\GameManager.h"

#include "Game\World.h"
#include "Game\Region.h"
#include "Game\Field.h"
#include "Game\Character.h"
#include "Game\Connection.h"

inline void pair(Region* _region1, int _direction, Region* _region2) { _region1->neighbours[_direction] = _region2; if (_region2) _region2->neighbours[reverse(_direction)] = _region1; };
//std::cout << "\t\t\t>Pairing " << _region1->x << ":" << _region1->y << " -> " << _region2->x << ":" << _region2->y << " ~ " << _direction << " (" << reverse(_direction) << ")\n"; };

/*Creates a group for the leader and the leader's region web.*/
Group::Group(boost::shared_ptr<Character> _leader)
	: disposed(false),
	  leader(_leader)
{
	//Calculate current region index
	int center_x = leader->Location().x / 512; int center_y = leader->Location().y / 512;

	//Create region web
	Region** regions = new Region*[(4 * VISION_RADIUS + 1) * (4 * VISION_RADIUS + 1)];
	for (int row = 0; row < 4 * VISION_RADIUS + 1; ++row)
		for (int column = 0; column < 4 * VISION_RADIUS + 1; ++column)
		{
			//Create new field at current location (all neighbours are set to 0 by default in constructor)
			regions[row * (4 * VISION_RADIUS + 1) + column] = new Region(center_x + column - 2 * VISION_RADIUS, center_y + row - 2 * VISION_RADIUS, leader->world);

			//There is a top-right neighbour
			if (row != 0 && column != 4 * VISION_RADIUS + 1 - 1) { pair(regions[row * (4 * VISION_RADIUS + 1) + column], 1, regions[(row - 1) * (4 * VISION_RADIUS + 1) + (column + 1)]); }

			//There is a top neighbour
			if (row != 0) { pair(regions[row * (4 * VISION_RADIUS + 1) + column], 2, regions[(row - 1) * (4 * VISION_RADIUS + 1) + column]); }

			//There is a top-left neighbour
			if (row != 0 && column != 0) { pair(regions[row * (4 * VISION_RADIUS + 1) + column], 3, regions[(row - 1) * (4 * VISION_RADIUS + 1) + column - 1]); }

			//There is a left neighbour
			if (column != 0) { pair(regions[row * (4 * VISION_RADIUS + 1) + column], 4, regions[row * (4 * VISION_RADIUS + 1) + column - 1]); }
		}

	//Set leader's region to the middle of the region web
	leader->region = regions[2 * VISION_RADIUS * (4 * VISION_RADIUS + 1) + 2 * VISION_RADIUS];

	//Get data of fields
	leader->region->Broadcast_EnterData(leader);

	//Send enter for all chaarcters
	leader->Broadcast_Enter();

	leader->region->field->characters.Add(leader);

	thread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&Group::Update, this)));
}

void Group::Update()
{
	float elapsed_time;
	boost::posix_time::ptime last_update = boost::posix_time::second_clock::local_time();
	do
    {
		//std::cout << thread->get_id();
		elapsed_time = (boost::posix_time::second_clock::local_time() - last_update).total_seconds();
		last_update = boost::posix_time::second_clock::local_time();

		characters.Process_Adding(this);

		Region const * center = leader->region;

		//Update leader
		leader->Update(elapsed_time);
		if (!leader->world->Loaded(*leader->region))
			leader->region->field->Process();

		//Update other members
		BOOST_FOREACH(boost::shared_ptr<Character> character, characters.data.list)
		{
			character->Update(elapsed_time);

			//TODO additional check if already processed field in this iteration
			if (!leader->world->Loaded(*character->region))
				character->region->field->Process();
		}

		//Update web if leader moved
		if (leader->State() & Unit_State::REGION_MOVING)
		{
			std::cout << "Leader moving..\n";
			//Add and delete regions
			Update_Center(center, leader->region);

			//Check if anyone is leaving group with the new region web
			BOOST_FOREACH(boost::shared_ptr<Character> character, characters.data.list)
			{
				if (VISION_RADIUS < abs(leader->region->x - character->region->x) || VISION_RADIUS < abs(leader->region->y - character->region->y))
				{
					characters.Remove(character);
					character->group = boost::shared_ptr<Group>(new Group(character));
				}
			}
		}
		else
			//Check other characters if they leave group
			BOOST_FOREACH(boost::shared_ptr<Character> character, characters.data.list)
			{
				if (character->State() & Unit_State::REGION_MOVING)
					if (VISION_RADIUS < abs(leader->region->x - character->region->x) || VISION_RADIUS < abs(leader->region->y - character->region->y))
					{
						characters.Remove(character);
						character->group = boost::shared_ptr<Group>(new Group(character));
					}
			}

		characters.Process_Removing(this);

		boost::this_thread::sleep(boost::posix_time::milliseconds(GROUP_UPDATE_INTERVAL));
	} while (true);
}

void Group::Update_Center(Region const * _old, const Region const * _new)
{
	//std::cout << ">Updating center " << _old->x << ":" << _old->y << " -> " << _new->x << ":" << _new->y << std::endl; 

	while (_old->x < _new->x)
		_old = Move_Center(_old, 0);
	while (_new->x < _old->x)
		_old = Move_Center(_old, 4);
	while (_new->y < _old->y)
		_old = Move_Center(_old, 2);
	while (_old->y < _new->y)
		_old = Move_Center(_old, 6);
}

Region const * Group::Move_Center(Region const * _center, const int& _direction)
{
	//std::cout << "\t>Moving center " << _center->x << ":" << _center->y << " ~> " << _direction << std::endl;

	static const int neighbour_direction[8] = { mod(0 + 1, 8), mod(1 + 1, 8), mod(2 + 1, 8), mod(3 + 1, 8), mod(4 + 1, 8), mod(5 + 1, 8), mod(6 + 1, 8), mod(7 + 1, 8) };
	static const int deleted_direction[8] = { mod(0 + 3, 8), mod(1 + 3, 8), mod(2 + 3, 8), mod(3 + 3, 8), mod(4 + 3, 8), mod(5 + 3, 8), mod(6 + 3, 8), mod(7 + 3, 8) };

	static const int new_offset[8][2] = { {+1, 0}, {0, 0}, {0, -1}, {0, 0}, {-1, 0}, {0, 0}, {0, 1}, {0, 0} };

	static const int next[8] = { mod(0 + 6, 8), 0, mod(2 + 6, 8), 0, mod(4 + 6, 8), 0, mod(6 + 6, 8), 0 };

	static const int neighbour_pairs[4][8][2] = { { { mod(0 + 2, 8), mod(0 + 1, 8) }, { 0, 0 }, { mod(2 + 2, 8), mod(2 + 1, 8) }, { 0, 0 }, { mod(4 + 2, 8), mod(4 + 1, 8) }, { 0, 0 }, { mod(6 + 2, 8), mod(6 + 1, 8) }, { 0, 0 } },
	                                              { { mod(0 + 3, 8), mod(0 + 2, 8) }, { 0, 0 }, { mod(2 + 3, 8), mod(2 + 2, 8) }, { 0, 0 }, { mod(4 + 3, 8), mod(4 + 2, 8) }, { 0, 0 }, { mod(6 + 3, 8), mod(6 + 2, 8) }, { 0, 0 } },
												  { { mod(0 + 4, 8), 0 }, { 0, 0 }, { mod(2 + 4, 8), 0 }, { 0, 0 }, { mod(4 + 4, 8), 0 }, { 0, 0 }, { mod(6 + 4, 8), 0 }, { 0, 0 } },
												  { { mod(0 + 5, 8), mod(0 + 6, 8) }, { 0, 0 }, { mod(2 + 5, 8), mod(2 + 6, 8) }, { 0, 0 }, { mod(4 + 5, 8), mod(4 + 6, 8) }, { 0, 0 }, { mod(6 + 5, 8), mod(6 + 6, 8) }, { 0, 0 } } };

	static const int deleted_pairs[3][8][2] =   { { { mod(0 + 1, 8), mod(0 + 5, 8) }, { 0, 0 }, { mod(2 + 1, 8), mod(2 + 5, 8) }, { 0, 0 }, { mod(4 + 1, 8), mod(4 + 5, 8) }, { 0, 0 }, { mod(6 + 1, 8), mod(6 + 5, 8) }, { 0, 0 } },
												  { { mod(0 + 0, 8), mod(0 + 4, 8) }, { 0, 0 }, { mod(2 + 0, 8), mod(2 + 4, 8) }, { 0, 0 }, { mod(4 + 0, 8), mod(4 + 4, 8) }, { 0, 0 }, { mod(6 + 0, 8), mod(6 + 4, 8) }, { 0, 0 } },
												  { { mod(0 + 7, 8), mod(0 + 3, 8) }, { 0, 0 }, { mod(2 + 7, 8), mod(2 + 3, 8) }, { 0, 0 }, { mod(4 + 7, 8), mod(4 + 3, 8) }, { 0, 0 }, { mod(6 + 7, 8), mod(6 + 3, 8) }, { 0, 0 } } };

	if (_direction % 2)
	{
		_center = Move_Center(_center, mod(_direction - 1, 8));
		_center = Move_Center(_center, mod(_direction + 1, 8));
		return _center;
	}
	else
	{
		Region* neighbour = _center->neighbours[neighbour_direction[_direction]]; for(int distance = 1; distance < 2 * VISION_RADIUS; ++distance) neighbour = neighbour->neighbours[neighbour_direction[_direction]];
		Region* deleted = _center->neighbours[deleted_direction[_direction]]; for(int distance = 1; distance < 2 * VISION_RADIUS; ++distance) deleted = deleted->neighbours[deleted_direction[_direction]];

		Region** regions = new Region*[4 * VISION_RADIUS + 1];
		for (int current = 0; current < 4 * VISION_RADIUS + 1; ++current)
		{
			//Create region
			regions[current] = new Region(neighbour->x + new_offset[_direction][0], neighbour->y + new_offset[_direction][1], leader->world);

			//Set the 4 neighbour pointer
			if (current != 0) pair(regions[current], neighbour_pairs[0][_direction][0], neighbour->neighbours[ neighbour_pairs[0][_direction][1] ]);
			if (current != 0) pair(regions[current], neighbour_pairs[1][_direction][0], neighbour->neighbours[ neighbour_pairs[1][_direction][1] ]);
			pair(regions[current], neighbour_pairs[2][_direction][0], neighbour);
			if (current != 4 * VISION_RADIUS) pair(regions[current], neighbour_pairs[3][_direction][0], neighbour->neighbours[ neighbour_pairs[3][_direction][1] ]);

			neighbour = neighbour->neighbours[next[_direction]];

			//Set the 3 neighbour to null pointer
			if (current != 0) { //std::cout << "Setting " << deleted->neighbours[ deleted_pairs[0][_direction][0] ]->x << ":" << deleted->neighbours[ deleted_pairs[0][_direction][0] ]->y << " ~ " << deleted_pairs[0][_direction][1] <<
				//" -> " << deleted->neighbours[ deleted_pairs[0][_direction][0] ]->neighbours[ deleted_pairs[0][_direction][1] ]->x << ":" << deleted->neighbours[ deleted_pairs[0][_direction][0] ]->neighbours[ deleted_pairs[0][_direction][1] ]->y << std::endl;

			deleted->neighbours[ deleted_pairs[0][_direction][0] ]->neighbours[ deleted_pairs[0][_direction][1] ] = 0;}

			//std::cout << "Setting " << deleted->neighbours[ deleted_pairs[1][_direction][0] ]->x << ":" << deleted->neighbours[ deleted_pairs[1][_direction][0] ]->y << " ~ " << deleted_pairs[1][_direction][1] <<
			//	" -> " << deleted->neighbours[ deleted_pairs[1][_direction][0] ]->neighbours[ deleted_pairs[1][_direction][1] ]->x << ":" << deleted->neighbours[ deleted_pairs[1][_direction][0] ]->neighbours[ deleted_pairs[1][_direction][1] ]->y << std::endl;

			deleted->neighbours[ deleted_pairs[1][_direction][0] ]->neighbours[ deleted_pairs[1][_direction][1] ] = 0;

			if (current != 4 * VISION_RADIUS) { //std::cout << "Setting " << deleted->neighbours[ deleted_pairs[2][_direction][0] ]->x << ":" << deleted->neighbours[ deleted_pairs[2][_direction][0] ]->y << " ~ " << deleted_pairs[2][_direction][1] <<
				//" -> " << deleted->neighbours[ deleted_pairs[2][_direction][0] ]->neighbours[ deleted_pairs[2][_direction][1] ]->x << ":" << deleted->neighbours[ deleted_pairs[2][_direction][0] ]->neighbours[ deleted_pairs[2][_direction][1] ]->y << std::endl;

			deleted->neighbours[ deleted_pairs[2][_direction][0] ]->neighbours[ deleted_pairs[2][_direction][1] ] = 0; }

			//Delete region
			//TODO DISPOSE?!

			Region* tmp = deleted;
			deleted = deleted->neighbours[ next[_direction] ];
			delete tmp;
		}

		return _center->neighbours[_direction];
	}
}

void Group::ProcessAdding(boost::shared_ptr<Character> _character)
{
	//Teleport to leader

	//Broadcast adding
}

void Group::ProcessRemoving(boost::shared_ptr<Character> _character)
{
	//Broadcast leaving
}


void Group::Dispose()
{
	if (disposed) return; disposed = true;

	#ifdef LOGGING
	Logger::Write(5, "\t>Disposing Group..");
	#endif
}

Group::~Group()
{
	#ifdef LOGGING
	Logger::Write(4, "\t>Group destructor..");
	#endif
}
