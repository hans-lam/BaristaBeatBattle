#include "data_manager.hpp"

const string PATH_TO_SAVE_FILE = "../../../data/persistence/";

void DataManager::write_data(std::string file_name, int current_level, vector<Character*> character_datas) {

	StringBuffer s;
	Writer<StringBuffer> writer(s);

	writer.StartObject();
	write_general_game_data(&writer, current_level);
	write_party_members(&writer, character_datas);
	writer.EndObject();

	handle_file_write(file_name, s.GetString());
}

void DataManager::write_general_game_data(Writer<StringBuffer>* writer, int current_level) {
	writer->Key("current_level");
	writer->Int(current_level);
}

void DataManager::write_party_members(Writer<StringBuffer>* writer, vector<Character*> character_datas) {
	writer->Key("party_member_data");
	writer->StartArray();
	for (Character* character : character_datas) {
		writer->StartObject();
		writer->Key("name");
		writer->String(character->get_name().c_str());
		writer->Key("level");
		writer->Int(character->level);
		writer->Key("curr_experience");
		writer->Int(character->curr_experience_points);

		CharacterStatSheet* curr_sheet = character->get_character_stat_sheet();

		writer->Key("max_health");
		writer->Int(curr_sheet->get_max_health());
		writer->Key("strength");
		writer->Int(curr_sheet->get_strength());
		writer->Key("magic");
		writer->Int(curr_sheet->get_magic());
		writer->Key("defense");
		writer->Int(curr_sheet->get_defense());
		writer->Key("magic_resist");
		writer->Int(curr_sheet->get_magic_resist());
		writer->Key("speed");
		writer->Int(curr_sheet->get_speed());

		writer->EndObject();
	}
	writer->EndArray();
}

void DataManager::read_data(string file_name) {
	Document doc = handle_file_read(file_name);

	if (doc.HasMember("current_level") && doc["current_level"].IsInt()) {
		int current_level = doc["current_level"].GetInt();
		std::cout << "Current Level: " << current_level << std::endl;
	}

	if (doc.HasMember("party_member_data")
		&& doc["party_member_data"].IsArray()) {
		const Value& party_member_data = doc["party_member_data"];
		std::cout << "Party Member Data: \n";	

		

		for (rapidjson::Value::ConstValueIterator itr = party_member_data.Begin(); itr != party_member_data.End(); ++itr) {
			const Value& party_member = *itr;
			string party_member_name = party_member["name"].GetString();
			int party_member_level = party_member["level"].GetInt();
			int party_member_curr_experience = party_member["curr_experience"].GetInt();
			int party_member_max_health = party_member["max_health"].GetInt();
			int party_member_strength = party_member["strength"].GetInt();
			int party_member_magic = party_member["magic"].GetInt();
			int party_member_defense = party_member["defense"].GetInt();
			int party_member_magic_resist = party_member["magic_resist"].GetInt();
			int party_member_speed = party_member["speed"].GetInt();

			std::cout << "  Found the data for: " << party_member_name << std::endl;
			std::cout << "    Level: " << party_member_level << std::endl;
			std::cout << "    curr_experience: " << party_member_curr_experience << std::endl;
			std::cout << "    max_health: " << party_member_max_health << std::endl;
			std::cout << "    strength: " << party_member_strength << std::endl;
			std::cout << "    magic: " << party_member_magic << std::endl;
			std::cout << "    defense: " << party_member_defense << std::endl;
			std::cout << "    magic_resist: " << party_member_magic_resist << std::endl;
			std::cout << "    speed: " << party_member_speed << std::endl;

			CharacterStatSheet* new_stat_sheet = new CharacterStatSheet(party_member_name,
				party_member_max_health,
				party_member_strength,
				party_member_magic,
				party_member_defense,
				party_member_magic_resist,
				party_member_speed);
			Character* new_character_data = new Character(party_member_name, new_stat_sheet);
			new_character_data->level = party_member_level;
			new_character_data->curr_experience_points = party_member_curr_experience;

			CharacterData registry_data = CharacterData();
			registry_data.characterData = new_character_data;

			registry.characterDatas.emplace(Entity(), registry_data);


		}
		std::cout << std::endl;

	}

}


void DataManager::handle_file_write(string file_name, string json_data) {
	std::cout << "Writing to file " << file_name << ":  \n " << json_data << std::endl;

	ofstream file(PATH_TO_SAVE_FILE + file_name);
	if (file.is_open()) {
		file << json_data << std::endl;
	}
	else {
		std::cout << "Error reading file." << std::endl;
	}
	file.close();
}


Document DataManager::handle_file_read(string file_name) {
	string file_location = PATH_TO_SAVE_FILE + file_name;
	FILE* fp = fopen(file_location.c_str(), "rb");
	if (!fp) {
		std::cerr << "Error: unable to open file"
			<< std::endl;
	}

	// Read the file into a buffer 
	char readBuffer[65536];
	FileReadStream is(fp, readBuffer,
		sizeof(readBuffer));

	// Parse the JSON document 
	Document doc;
	doc.ParseStream(is);

	// Check if the document is valid 
	if (doc.HasParseError()) {
		std::cerr << "Error: failed to parse JSON document"
			<< std::endl;
	}

	fclose(fp);

	return doc;
}


DataManager* data_manager = new DataManager();