#ifndef MANAGER_DATABASE_MANAGER_H
#define MANAGER_DATABASE_MANAGER_H

#define DATABASE "database.db"
#define DATABASE_BACKUP "database_backup.db"
#define DATABASE_BACKUP_TIME 1 * 60

#include <fstream>

#include <boost\asio.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\filesystem.hpp>
#include <boost\thread.hpp>

#include "Core\Message.hpp"

class server;
class Connection;

struct Node
{
	char* name;
	char* pass;
	boost::shared_ptr<Connection> connection;

	Node* next;
	Node(const char* _name, const size_t _nameLength, const char* _pass, const size_t _passLength)
		: connection(0)
	{
		name = new char[_nameLength];
		int i = 0;
		do
		{
			name[i] = _name[i];
			++i;
		}while(_name[i] != '\0');
		name[i] = '\0';

		pass = new char[_passLength];
		i = 0;
		do
		{
			pass[i] = _pass[i];
			++i;
		}while(_pass[i] != '\0');
		pass[i] = '\0';
	}
};

struct AbcTable_1
{
	Node* nodes[26];
	boost::shared_mutex mutex;

	AbcTable_1()
	{
		for (int i = 0; i < 26; ++i) nodes[i] = 0;
	}
};
struct AbcTable_2
{
	AbcTable_1* nextLevel[26];

	AbcTable_2()
	{
		for (int i = 0; i < 26; ++i) nextLevel[i] = new AbcTable_1();
	}
};
struct AbcTable_3
{
	AbcTable_2* nextLevel[26];

	AbcTable_3()
	{
		for (int i = 0; i < 26; ++i) nextLevel[i] = new AbcTable_2();
	}
};
struct AbcTable_4
{
	AbcTable_3* nextLevel[26];

	AbcTable_4()
	{
		for (int i = 0; i < 26; ++i) nextLevel[i] = new AbcTable_3();
	}
};

class DatabaseManager
{
public:
	DatabaseManager();

	void Run();
	bool Authenticate(const std::string& _name, const std::string& _pass, boost::shared_ptr<Connection> _connection);
	bool Registrate(const std::string& _name, const std::string& _pass);
	bool Remove(const std::string& _name);

private:
	AbcTable_4* abcTable;

	std::ofstream database;
	boost::shared_mutex database_mutex;

    boost::asio::io_service io_service;
	boost::shared_ptr<boost::thread> thread;

	boost::asio::deadline_timer backupTimer;
	boost::shared_ptr<boost::thread> backupThread;

	void Load();
	void Backup();
	void BackupCallback(boost::asio::deadline_timer* _timer, const boost::system::error_code& _error);
	void Shutdown(std::ifstream& _file);

};
#endif