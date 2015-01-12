#include "Core\Macro.h"
#include "Game\Field_Generated.h"

#include <boost\bind.hpp>
#include <boost\thread.hpp>
#include <boost\foreach.hpp>
#include <boost\random\random_device.hpp>
#include <boost\random\uniform_int_distribution.hpp>

#include "Game\Character.h"

Field_Generated::Field_Generated()
	: disposed(false)
{
	#ifdef LOGGING
	//Logger::Write(7, "\t\t\t>Generating Field..");
	#endif // LOGGING

	boost::random::random_device generator;
	boost::random::uniform_int_distribution<> id(0, 123123123);
    boost::random::uniform_int_distribution<> coordinate(0, 512);

	int number = 4;
	for(int current = 0; current < number * number; ++current)
		objects.push_back(new Object(coordinate(generator), coordinate(generator), id(generator)));
}

void Field_Generated::Process()
{
	characters.Process_Removing(this);

	Process_Messages();

	characters.Process_Adding(this);
}

void Field_Generated::ProcessAdding(boost::shared_ptr<Character> _character)
{

}

void Field_Generated::ProcessRemoving(boost::shared_ptr<Character> _character)
{

}


void Field_Generated::Dispose()
{
	if (disposed) return; disposed = true;

	#ifdef LOGGING
	Logger::Write(5, "\t>Disposing generated field.");
	#endif

	BOOST_FOREACH(Object* object, objects)
	{
		delete object;
	}

	//Dispose characters?
}

Field_Generated::~Field_Generated()
{
	#ifdef LOGGING
	Logger::Write(4, "\t>Generated field destructor..");
	#endif
}



