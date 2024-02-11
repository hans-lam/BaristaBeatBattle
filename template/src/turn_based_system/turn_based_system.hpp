#include <vector>


class TurnBasedSystem {
public:

	void init();

	void step();

	void process_character_action(Ability ability, Character caller, vector<Character> recipients);

	void game_over();

private:
	std::vector<Character> party_members;

	void construct_party();
};