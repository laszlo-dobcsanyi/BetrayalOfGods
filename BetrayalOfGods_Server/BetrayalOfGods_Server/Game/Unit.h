#ifndef GAME_UNIT_H
#define GAME_UNIT_H

#include <string>
#include <boost\shared_ptr.hpp>

class server;
class World;
class Region;

extern server* Server;

enum Unit_State
{
	REGION_MOVING	= 0x00000001,
	DEAD			= 0x00000010
};

struct Vector2
{
	float x, y;

	Vector2(float _x, float _y) : x(_x), y(_y) { }
};

class Unit
{
    public:
        const std::string& GetName() { return name; }	
		std::string GetData();

		World* world;
		Region* region;

		virtual uint8_t Update(const float& _elapsedtime) = 0;

		void Rotate(const float& _direction);
		void Stop();

		inline const uint8_t State() { return state; };
		inline const Vector2& Location() { return location; };

		virtual void Dispose() = 0;

	protected:
		bool disposed;

		uint8_t state;

		int id;
		int icon_id;
		Vector2 location;
		Vector2 move_vector;
		float direction;
		float speed;
		bool moving;

        std::string name;

		Unit(const int _id, const std::string& _name);

		virtual void Broadcast_Location() = 0;
};

#endif
