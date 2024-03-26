#pragma once

// internal
#include "common.hpp"

// stlib
#include <vector>
#include <random>
#include <map>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

#include "render_system.hpp"
#include "turn_based_system/turn_based_system.hpp"
#include "stage_system/stage_system.hpp"
#include "stage_system/main_menu/main_menu_system.hpp"
#include "stage_system/overworld/overworld_system.hpp"
#include "stage_system/cutscene/cutscene_system.hpp"
#include "stage_system/combat/combat_system.hpp"
#include "stage_system/combat/minigame_system.hpp"

// Container for all our entities and game logic. Individual rendering / update is
// deferred to the relative update() methods
class WorldSystem
{
public:
	WorldSystem();

	// Creates a window
	GLFWwindow* create_window();

	// starts the game
	void init(RenderSystem* renderer, TurnBasedSystem* turn_based_arg,
		StageSystem* stage_system_arg, MainMenuSystem* main_menu_system_arg,
		OverworldSystem* overworld_system_arg, CutSceneSystem* cutscene_system_arg,
		CombatSystem* combat_system_arg, MinigameSystem* minigame_system_arg);

	// Releases all associated resources
	~WorldSystem();

	// Steps the game ahead by ms milliseconds
	bool step(float elapsed_ms);

	// Check for collisions
	void handle_collisions();

	// Should the game be over ?
	bool is_over()const;

	// check if player is in world bounds
	bool player_in_bounds(Motion* motion, bool is_x);

	void handle_mini(int bpm);
private:
	// setting fps
	void set_fps(float elapsed_ms_since_last_update);

	// setting music
	void set_music(StageSystem::Stage curr_stage);

	// Input callback functions
	void on_key(int key, int, int action, int mod);

	// restart level
	void restart_game();

	// change stage 
	void change_stage(int level);

	// OpenGL window handle
	GLFWwindow* window;

	// Game state
	RenderSystem* renderer;
	TurnBasedSystem* turn_based;
	StageSystem* stage_system;
	MainMenuSystem* main_menu_system;
	OverworldSystem* overworld_system;
	CutSceneSystem* cutscene_system;
	CombatSystem* combat_system;
	MinigameSystem* minigame_system;

	float current_speed;
	float player_speed;
	float next_eagle_spawn;
	float next_bug_spawn;
	float next_enemy_spawn;
	Entity player_chicken;
	int stage;

	// music references
	Mix_Music* main_menu_music;
	Mix_Music* background_music;
	Mix_Music* cutscene_music;
	Mix_Music* turn_based_music;
	Mix_Music* minigame_music;

	// Sound Effects
	Mix_Chunk* change_selection_effect;
	Mix_Chunk* chicken_dead_sound;
	Mix_Chunk* chicken_eat_sound;
	Mix_Chunk* attack_sound;

	// music to stage mapping
	std::map<StageSystem::Stage, Mix_Music*> stage_music_map;

	// C++ random number generator
	std::default_random_engine rng;
	std::uniform_real_distribution<float> uniform_dist; // number between 0..1
	std::uniform_real_distribution<float> wide_dist; // number between -1..1
};
