#pragma once

#include "character_system.hpp"
#include <vector>
#include <string>

// I HAVE TO FORWARD DELCARE THIS FOR SOME REASON???? I AM SO CONFUSED
class Character;

class Ability{
public:
	Ability(unsigned int power, std::string name, bool is_physical, bool is_area_of_effect);

	unsigned int get_power();
	std::string get_ability_name();
	bool get_is_physical();
	bool get_is_area_of_effect();

	void process_ability(Character* caller, Character* target);

private:
	int power;
	std::string ability_name;
	bool is_physical;
	bool is_area_of_effect;
};


class SupportAbility : public Ability {
public:
	SupportAbility(unsigned int potency, std::string ability_name, bool is_healing, bool is_area_of_effect);

	void process_ability(Character* caller, Character* target);

	unsigned get_potency();
	bool get_is_healing();

private:
	unsigned int potency;
	bool is_healing;
};

/*
enum BeatScore
{
	BEAT_PERFECT = 2, BEAT_HIT = 1, BEAT_MISS = 0
};

class BeatInstance {
public: 
	int potency;
	BeatScore beat_score;
};
*/