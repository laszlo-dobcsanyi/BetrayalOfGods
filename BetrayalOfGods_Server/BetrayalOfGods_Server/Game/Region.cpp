#include "Core\Macro.h"
#include "Game\Region.h"

#include <boost\foreach.hpp>
#include <boost\lexical_cast.hpp>

#include "Game\World.h"
#include "Game\Field.h"
#include "Game\Character.h"
#include "Game\Connection.h"

Region::Region(const int _x, const int _y, World* _world)
	: x(_x),
	  y(_y),
	  field(_world->GetField(_x, _y))
{
	for(int current = 0; current < 8; ++current)
		neighbours[current] = 0;

	#ifdef LOGGING
	//Logger::Write(7, "\t\t>Creating region @ [" + boost::lexical_cast<std::string>(x) + ":" + boost::lexical_cast<std::string>(y) + "]..");
	#endif // LOGGING
}

void Region::BroadcastMessage(boost::shared_ptr<Message> _msg)
{
	field->BroadcastMessage(_msg);

	for(int direction = 0; direction < 4; ++direction)
	{
		Region* current = this;
		int counter = VISION_RADIUS;

		while ((0 < counter) && (current->neighbours[direction * 2]))
		{
			current = current->neighbours[direction * 2];
			current->field->BroadcastMessage(_msg);
			--counter;
		}

		neighbours[direction * 2 + 1]->BroadcastMessage_Iteration(VISION_RADIUS, direction, _msg);
	}
}

void Region::BroadcastMessage_Iteration(const int& _distance, const int& _direction, boost::shared_ptr<Message> _msg)
{
	field->BroadcastMessage(_msg);

	Region* current = this;
	int counter = _distance;

	while ((0 < counter) && (current->neighbours[_direction * 2]))
	{
		current = current->neighbours[_direction * 2];
		current->field->BroadcastMessage(_msg);
		counter--;
	}

	current = this;
	counter = _distance;
	while ((0 < counter) && (current->neighbours[((_direction + 1) * 2) % 8]))
	{
		current = current->neighbours[((_direction + 1) * 2) % 8];
		current->field->BroadcastMessage(_msg);
		counter--;
	}

	if (1 < _distance) neighbours[_direction * 2 + 1]->BroadcastMessage_Iteration(_distance - 1, _direction, _msg);
}

void Region::Broadcast_EnterData(boost::shared_ptr<Character> _character)
{
	field->Broadcast_Enter(_character);

	for(int direction = 0; direction < 4; ++direction)
	{
		Region* current = this;
		for(int distance = 0; distance < VISION_RADIUS; ++distance)
		{
			current = current->neighbours[direction * 2];
			current->field->Broadcast_Enter(_character);
		}

		current = this;
		for(int diagonal = 0; diagonal < VISION_RADIUS; ++diagonal)
		{
			current = current->neighbours[direction * 2 + 1];
			current->field->Broadcast_Enter(_character);

			Region* current_diagonal = current;
			for(int distance = diagonal + 1; distance < VISION_RADIUS; ++distance)
			{
				current_diagonal = current_diagonal->neighbours[direction * 2];
				current_diagonal->field->Broadcast_Enter(_character);
			}
			
			current_diagonal = current;
			for(int distance = diagonal + 1; distance < VISION_RADIUS; ++distance)
			{
				current_diagonal = current_diagonal->neighbours[mod(direction * 2 + 2, 8)];
				current_diagonal->field->Broadcast_Enter(_character);
			}
		}
	}
}

void Region::Broadcast_LeaveData(boost::shared_ptr<Character> _character)
{

}


