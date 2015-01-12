#ifndef GAME_REGION_H
#define GAME_REGION_H

#define VISION_RADIUS 2

#include <boost\shared_ptr.hpp>

class World;
class Field;
class Message;
class Character;

class Region
{
public:
	int x, y;

	// 3 2 1
	// 4 X 0
	// 5 6 7
	Field* field;

	Region* neighbours[8];

	Region(const int _x, const int _y, World* _world);

	void BroadcastMessage(boost::shared_ptr<Message> _msg);

	void Broadcast_EnterData(boost::shared_ptr<Character> _character);
	void Broadcast_LeaveData(boost::shared_ptr<Character> _character);

private:
	void BroadcastMessage_Iteration(const int& _distance, const int& _direction, boost::shared_ptr<Message> _msg);
};

#endif