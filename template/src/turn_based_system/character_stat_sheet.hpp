#pragma once

class CharacterStatSheet {
public:

	CharacterStatSheet(unsigned int max_health_,
		unsigned int strength_,
		unsigned int magic_,
		unsigned int defense_,
		unsigned int magic_resist_,
		unsigned int speed_);

	void set_max_health(unsigned int max_health);
	void set_strength(unsigned int strength);
	void set_magic(unsigned int magic);
	void set_defense(unsigned int defense);
	void set_magic_resist(unsigned int magic_resist);
	void set_speed(unsigned int speed);

	unsigned int get_max_health();
	unsigned int get_strength();
	unsigned int get_magic();
	unsigned int get_defense();
	unsigned int get_magic_resist();
	unsigned int get_speed();

private:
	unsigned int max_health;
	unsigned int strength;
	unsigned int magic;
	unsigned int defense;
	unsigned int magic_resist;
	unsigned int speed;
};