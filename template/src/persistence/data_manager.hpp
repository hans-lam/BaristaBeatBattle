
#include "../turn_based_system/character_system.hpp"
#include "../tiny_ecs_registry.hpp"
#include "../stage_system/stage_system.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

#include "writer.h"
#include "stringbuffer.h"
#include "filereadstream.h"
#include "document.h"

using namespace rapidjson;
using namespace std;

// TUTORIALS FOLLOWED https://github.com/Tencent/rapidjson/blob/master/example/simplewriter/simplewriter.cpp
//					  https://www.geeksforgeeks.org/how-to-parse-an-array-of-objects-in-cpp-using-rapidjson/


class DataManager {
public:
	void write_data(string file_name, int current_level, vector<Character*> character_data, bool is_london_recruited);
	void read_data(string file_name, StageSystem* stage_system);
private:
	Document handle_file_read(string file_name);
	void handle_file_write(string file_name, string json_data);

	void write_general_game_data(Writer<StringBuffer>* writer, int current_level, bool is_london_recruited);
	void write_party_members(Writer<StringBuffer>* writer, vector<Character*> character_data);
};


extern DataManager* data_manager;