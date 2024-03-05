#pragma once

class CharacterStatSheet {
public:

	CharacterStatSheet(int max_health_,
		int strength_,
		int magic_,
		int defense_,
		int magic_resist_,
		int speed_);

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