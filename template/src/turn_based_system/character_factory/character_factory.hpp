#include "../character_system.hpp"


class CharacterFactory {

	Character construct_chai();

	Character construct_earl();

	Character construct_americano();

	Character construct_enemy(int level);
};

CharacterFactory character_factory;