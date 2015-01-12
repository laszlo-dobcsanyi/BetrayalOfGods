#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <vector>
#include <sstream>

#include <boost\chrono.hpp>
#include <boost\thread.hpp>
#include <boost\shared_ptr.hpp>

#include <Core\ListProcessable.hpp>
#include <Core\ListLockable.hpp>

class server;
class Message;
class Character;

struct Object
{
	int x, y, id;

	Object(const int& _x, const int& _y, const int& _id) : x(_x), y(_y), id(_id) { };

	inline const std::string GetData() const { std::stringstream builder; builder << x << ';' << y << ';' << id; return builder.str(); };
};

class Field
	: public ListProcessable_Callback< boost::shared_ptr<Character> >
{
public:
	ListProcessable<boost::shared_ptr<Character> > characters;

	virtual void Process() = 0;

	virtual void ProcessAdding(boost::shared_ptr<Character> _character) = 0;
	virtual void ProcessRemoving(boost::shared_ptr<Character> _character) = 0;

	inline void BroadcastMessage(boost::shared_ptr<Message> _msg) { if (0 < characters.data.number) messages.Add(_msg); }

	virtual void Broadcast_Enter(boost::shared_ptr<Character> _character);
	virtual void Broadcast_Leave(boost::shared_ptr<Character> _character);

protected:
	ListLockable<boost::shared_ptr<Message> > messages;
	std::vector<Object*> objects;

	void Process_Messages();
};

#endif
