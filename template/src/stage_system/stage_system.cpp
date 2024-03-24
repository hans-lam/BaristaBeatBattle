#include "stage_system.hpp"

StageSystem::StageSystem()
{

}

StageSystem::set_stage(stage target)
{
	current_stage = target; 

	switch (target) {
	case main_menu:
		set_main_menu();
	case overworld:
		set_overworld();
	case cutscene:
		set_cutscene();
	case turn_based:
		set_turn_based();
	case minigame:
		set_minigame();
	}
}

StageSystem::set_main_menu()
{

}

StageSystem::set_overworld()
{

}

StageSystem::set_cutscene()
{

}

StageSystem::set_turn_based()
{

}

StageSystem::set_minigame()
{

}