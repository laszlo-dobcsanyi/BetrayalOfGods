#include "Core\Macro.h"
#include "Game\Person.h"

#include "Core\Server.h"
#include "Game\Unit.h"

Person::Person(const std::string& _name)
	: Unit(0, _name)
{

}

void Person::Dispose()
{
	if (disposed) return; disposed = true;

	#ifdef LOGGING
	Logger::Write(5, "\t>Disposing person..");
	#endif
}
