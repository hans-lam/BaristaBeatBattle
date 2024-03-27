#pragma once
#include "ability_factory.hpp"

const int PHYSCIAL_BASIC_ATTACK_POWER = 5;

Ability* AbilityFactory::construct_pour_it() {

	std::vector<BeatInstance> beats;
	beats.push_back(BeatInstance(2, 2));
	beats.push_back(BeatInstance(2, 2));
	beats.push_back(BeatInstance(5, 3));
	beats.push_back(BeatInstance(2, 2));
	beats.push_back(BeatInstance(2, 2));
	beats.push_back(BeatInstance(5, 3));

	Ability* pour_it = new BeatAbility("pour", true, false, beats);

	return pour_it;
}

Ability* AbilityFactory::construct_basic_attack() {
	Ability* generic_basic_attack = new Ability(PHYSCIAL_BASIC_ATTACK_POWER, "Basic Attack", true, false);
	return generic_basic_attack;
}

Ability* AbilityFactory::construct_rest() {
	Ability* rest = new SupportAbility(10, "rest", true, false);
	return rest;
}

AbilityFactory* ability_factory = new AbilityFactory();