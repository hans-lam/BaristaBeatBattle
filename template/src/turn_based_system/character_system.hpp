#include "abilities.hpp"
#include <vector>
#include <string>

class Character {
public:
	Character(std::string name, CharacterStatSheet stats);

	void add_ability(Ability ability);

	void restore_health_to_full();

	int get_current_health_points();

	CharacterStatSheet get_character_stat_sheet();


private:
	std::string character_name;
	CharacterStatSheet stats;
	std::vector<Ability> abilities;

	int current_health_points;
};

class CharacterStatSheet {
public:

	CharacterStatSheet(int max_health, int strength, int magic, int defense, int magic_resist, int speed);

	void set_max_health(int max_health);
	void set_strength(int strength);
	void set_magic(int magic);
	void set_defense(int defense);
	void set_magic_resist(int magic_resist);
	void set_speed(int speed);

	int get_max_health();
	int get_strength();
	int get_magic();
	int get_defense();
	int get_magic_resist();
	int get_speed();

private:
	int max_health;
	int strength;
	int magic;
	int defense;
	int magic_resist;
	int speed;
};