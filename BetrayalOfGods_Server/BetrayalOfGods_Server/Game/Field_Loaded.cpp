#include "Core\Macro.h"
#include "Game\Field_Loaded.h"

#include <boost\foreach.hpp>

#include "Game\Character.h"

Field_Loaded::Field_Loaded(const std::string& _filename)
{
    #ifdef LOGGING
    Logger::Write(4, "\t\t\t>Loading Field from " + _filename + " .."); 
    #endif // LOGGING
}

void Field_Loaded::Process()
{
	characters.Process_Removing(this);

	Process_Messages();

	characters.Process_Adding(this);
}

void Field_Loaded::ProcessAdding(boost::shared_ptr<Character> _character)
{

}

void Field_Loaded::ProcessRemoving(boost::shared_ptr<Character> _character)
{

}


