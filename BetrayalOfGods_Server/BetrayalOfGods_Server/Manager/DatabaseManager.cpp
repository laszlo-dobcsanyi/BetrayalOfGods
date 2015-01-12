#include "Core\Macro.h"
#include "Manager\DatabaseManager.h"

#include <boost\algorithm\string.hpp>
#include <boost\date_time\posix_time\posix_time.hpp>

#include "Game\Connection.h"

DatabaseManager::DatabaseManager()
	: backupTimer(io_service, boost::posix_time::seconds(DATABASE_BACKUP_TIME))
{
	#ifdef LOGGING
	Logger::Write(4, "\t\t>DatabaseManager allocating AbcTable..");
	#endif //LOGGING

	abcTable = new AbcTable_4();

	#ifdef LOGGING
	Logger::Write(4, "\t\t>DatabaseManager allocated!");
	#endif //LOGGING

	Load();

	#ifdef LOGGING
	Logger::Write(4, "\t>DatabaseManager created!");
	#endif //LOGGING
}

void DatabaseManager::Run()
{
	backupTimer.async_wait(boost::bind(&DatabaseManager::BackupCallback, this, &backupTimer, boost::asio::placeholders::error));
	io_service.run();
}


void DatabaseManager::Load()
{
	#ifdef LOGGING
	Logger::Write(4, "\t\t>Loading Database..");
	#endif //LOGGING

	std::ifstream dataFile;
	dataFile.open(DATABASE);

	std::string tmp;
	while(dataFile >> tmp)
	{
		std::vector<std::string> args;
		boost::split(args, tmp, boost::is_any_of(";"));
		
		Node* node = new Node(args[0].c_str(), args[0].size() + 1, args[1].c_str(), args[1].size() + 1);
		node->next = abcTable->nextLevel[args[0][0]-'a']->nextLevel[args[0][1]-'a']->nextLevel[args[0][2]-'a']->nodes[args[0][3]-'a'];
		abcTable->nextLevel[args[0][0]-'a']->nextLevel[args[0][1]-'a']->nextLevel[args[0][2]-'a']->nodes[args[0][3]-'a'] = node;
	}

	dataFile.close();

	#ifdef LOGGING
	Logger::Write(4, "\t\t>Database Loaded!");
	#endif //LOGGING
}

void DatabaseManager::BackupCallback(boost::asio::deadline_timer* _timer,const boost::system::error_code& _error)
{
	backupThread = boost::shared_ptr<boost::thread>(new boost::thread(boost::bind(&DatabaseManager::Backup, this)));

	_timer->expires_at(_timer->expires_at() + boost::posix_time::seconds(DATABASE_BACKUP_TIME));
	_timer->async_wait(boost::bind(&DatabaseManager::BackupCallback, this, _timer, boost::asio::placeholders::error));
}

void DatabaseManager::Backup()
{
	#ifdef LOGGING
	Logger::Write(4, "\t>Backing up data..");
	#endif //LOGGING

	boost::unique_lock<boost::shared_mutex> lock(database_mutex);

	database.close();

	remove(DATABASE_BACKUP);

	boost::filesystem::path databasePath = DATABASE;
	boost::filesystem::path backupPath = DATABASE_BACKUP;

    try
    {
        boost::filesystem::copy_file(databasePath, backupPath);
    } 
	catch (const boost::filesystem::filesystem_error& e)
    {
		#ifdef LOGGING
		Logger::Write(1, "#Error @ DatabaseManager Backup : " + boost::lexical_cast<std::string>(e.what()) + "!");
		#endif //LOGGING
    }

	database.open(DATABASE, std::ios_base::app);
	if(database.fail())
	{
		#ifdef LOGGING
		Logger::Write(1, "#Error @ DatabaseManager Backup : Open file ("DATABASE")!");
		#endif //LOGGING
		return;
	}

	lock.unlock();

	#ifdef LOGGING
	Logger::Write(4, "\t>Successful database backup!");
	#endif //LOGGING
}


bool DatabaseManager::Authenticate(const std::string& _name, const std::string& _pass, boost::shared_ptr<Connection> _connection)
{
	#ifdef LOGGING
	Logger::Write(7, "\t>Authenticating..");
	#endif //LOGGING

	const char* cname = _name.c_str();
	const char* cpass = _pass.c_str();

	boost::upgrade_lock<boost::shared_mutex> lock(abcTable->nextLevel[_name[0]-'a']->nextLevel[_name[1]-'a']->nextLevel[_name[2]-'a']->mutex);

	Node* p = abcTable->nextLevel[_name[0]-'a']->nextLevel[_name[1]-'a']->nextLevel[_name[2]-'a']->nodes[_name[3]-'a'];
	while(p != 0)
	{
		int i = 0;

		while (p->name[i] != '\0' && cname[i] != '\0' && p->name[i] == cname[i]) ++i;

		if (p->name[i] == '\0' && cname[i] == '\0')
		{
			i = 0;

			while (p->pass[i] != '\0' && cpass[i] != '\0' && p->pass[i] == cpass[i]) ++i;

			if (p->pass[i] == '\0' && cpass[i] == '\0' )
			{
				if (p->connection == 0)
				{
					#ifdef LOGGING
					Logger::Write(7, "\t>DatabaseManager adding Connection..");
					#endif

					p->connection = _connection;
				}
				else
				{
					#ifdef LOGGING
					Logger::Write(7, "\t>DatabaseManager adding  and removing Connections..");
					#endif

					p->connection->name_in_database = "\0";
					p->connection->Dispose();
					p->connection = _connection;
				}

				return true;
			}

			return false;
		}

		p = p->next;
	} 

	return false;
}

bool DatabaseManager::Registrate(const std::string& _name, const std::string& _pass)
{
	#ifdef LOGGING
	Logger::Write(7, "\t>Registrating..");
	#endif //LOGGING

	const char* cname = _name.c_str();
	const char* cpass = _pass.c_str();

	boost::upgrade_lock<boost::shared_mutex> lock(abcTable->nextLevel[_name[0]-'a']->nextLevel[_name[1]-'a']->nextLevel[_name[2]-'a']->mutex);
	Node* p = abcTable->nextLevel[_name[0]-'a']->nextLevel[_name[1]-'a']->nextLevel[_name[2]-'a']->nodes[_name[3]-'a'];

	while(p != 0)
	{
		int i = 0;

		while (p->name[i] != '\0' && cname[i] != '\0' && p->name[i] == cname[i]) ++i;

		if (p->name[i] == '\0' && cname[i] == '\0')
		{
			return false;
		}

		p = p->next;
	} 

	boost::upgrade_to_unique_lock<boost::shared_mutex> unique(lock);

	Node* node = new Node(cname, _name.size() + 1, cpass, _pass.size() + 1);
	node->next = abcTable->nextLevel[cname[0]-'a']->nextLevel[cname[1]-'a']->nextLevel[cname[2]-'a']->nodes[cname[3]-'a'];
	abcTable->nextLevel[cname[0]-'a']->nextLevel[cname[1]-'a']->nextLevel[cname[2]-'a']->nodes[cname[3]-'a'] = node;

	boost::unique_lock<boost::shared_mutex> database_lock(database_mutex);
	database << _name << ";" << _pass << '\n';

	return true;
}

bool DatabaseManager::Remove(const std::string& _name)
{
	int i;
	Node* p = abcTable->nextLevel[_name[0]-'a']->nextLevel[_name[1]-'a']->nextLevel[_name[2]-'a']->nodes[_name[3]-'a'];

	while(p != 0)
	{
		i = 0;
		while (p->name[i] != '\0' && _name[i] != '\0' && p->name[i] == _name[i]) ++i;

		if (p->name[i] == '\0' && _name[i] == '\0')
		{				
			p->connection.reset();
			return true;
		}

		p = p->next;
	}

	#ifdef LOGGING
	Logger::Write(1, "# Error @ DatabaseManager Remove!");
	Logger::Write(1, _name +" not found in DatabaseManager Nodes!" );
	#endif
	return false;
}

