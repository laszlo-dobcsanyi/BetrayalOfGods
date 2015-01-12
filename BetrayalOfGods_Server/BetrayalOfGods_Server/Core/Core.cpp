#include "Core\Macro.h"
#include "Core\Server.h"
#include "Manager\ConnectionManager.h"

void Banner();
bool HandleCommand();

server* Server;

int main()
{
	Banner();

	Server = new server();

	while (true)
	{
		HandleCommand();
	}

	return 0;
}

void Banner()
{
	setlocale(LC_ALL, "hun");

	std::cout << "          ___      __                     __       ___  _____        __  \n";
	std::cout << "         / _ )___ / /________ ___ _____ _/ / ___  / _/ / ___/__  ___/ /__\n";
	std::cout << "        / _  / -_) __/ __/ _ `/ // / _ `/ / / _ \\/ _/ / (_ / _ \\/ _  (_-<\n";
	std::cout << "       /____/\\__/\\__/_/  \\_,_/\\_, /\\_,_/_/  \\___/_/   \\___/\\___/\\_,_/___/\n";
	std::cout << "                             /___/                                       \n";

}

bool HandleCommand()
{
	getchar();
	return true;
}
