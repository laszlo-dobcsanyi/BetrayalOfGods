#include "Core\Macro.h"
#include "Game\World.h"

#include <fstream>

#include <boost\bind.hpp>
#include <boost\foreach.hpp>
#include <boost\lexical_cast.hpp>

#include "Game\Group.h"
#include "Game\Region.h"
#include "Game\Character.h"
#include "Game\Field_Loaded.h"
#include "Game\Field_Generated.h"

World::World(const int& _id)
	: id(_id)
{
	#ifdef LOGGING
    Logger::Write(4, "\t>Loading World " + boost::lexical_cast<std::string>(_id) + "..");
    #endif // LOGGING

	std::ifstream world_file;

	std::string path;
	path.append("worlds\\"); path.append(boost::lexical_cast<std::string>(_id)); path.append(".data");
	world_file.open (path.c_str(), std::ifstream::in);
    if (world_file.fail())
    {
        #ifdef LOGGING
        Logger::Write(1, "#Error @ World constructor while loading world file!");
        #endif // LOGGING
        return;
    }

	int number;
	world_file >> width >> height;// >> number;

	areas = 0; //new Area[number];
	fields = new Field_Descriptor[width * height];

  //  while(number--)
  //  {
		//std::ifstream area_file;
		//std::string area_name;

		//world_file >> area_name;

		//std::string path;
		//path.append("worlds\\areas\\"); path.append(area_name); path.append(".data");
		//area_file.open (path.c_str(), std::ifstream::in);
		//if (area_file.fail())
		//{
		//	#ifdef LOGGING
		//	Logger::Write(1, "#Error @ World constructor while loading area file!");
		//	#endif // LOGGING
		//	return;
		//}

		//int current;
		//area_file >> current;

		//while (current--)
		//{
		//	int x, y;
		//	std::string file_name;

		//	area_file >> file_name >> x >> y;

		//	Field_Loaded* field = new Field_Loaded(file_name);

		//	areas[number].fields.push_back(field);
		//	fields[x + y * width].Loaded(field);
		//}

		//areas[number].Run();
  //  }

	int value;
	for (int row = 0; row < height; ++row)
		for (int column = 0; column < width; ++column)
		{
			int tmp;
			world_file >> value >> value >> value >> tmp;
			//if (!fields[column + row * width].field_loaded)
			fields[column + row * width].Generated(value);
		}
}

void Area::Update()
{
	float elapsed_time;
	boost::posix_time::ptime last_update = boost::posix_time::microsec_clock::local_time();
	do
    {
		elapsed_time = (boost::posix_time::microsec_clock::local_time() - last_update).total_milliseconds() / 1000.0;
		last_update = boost::posix_time::microsec_clock::local_time();

		BOOST_FOREACH(Field_Loaded* current, fields)
		{
			current->Process();
		}

        boost::this_thread::sleep(boost::posix_time::milliseconds(FIELD_LOADED_UPDATE_INTERVAL));
    } while (true);
}


Field* World::GetField(const int& _x, const int& _y)
{
	if (fields[_x + _y * width].field_loaded) return fields[_x + _y * width].field_loaded;
	return new Field_Generated();
}
