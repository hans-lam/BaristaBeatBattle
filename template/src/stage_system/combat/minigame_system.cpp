#include "minigame_system.hpp"

MinigameSystem::MinigameSystem() :
	started(false)
{}

void MinigameSystem::init(StageSystem* stage_system_arg) {
	stage_system = stage_system_arg; 
	started = false;
}

void MinigameSystem::handle_minigame_key(int key, int action) {

}