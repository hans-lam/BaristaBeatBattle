#pragma once

// internal
#include "common.hpp"

// stlib
#include <vector>
#include <random>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

#include "render_system.hpp"
#include "turn_based_system/turn_based_system.hpp"

// Container for all our entities and game logic. Individual rendering / update is
// deferred to the relative update() methods
class WorldSystem
{
public:
	WorldSystem();

	// Creates a window
	GLFWwindow* create_window();

	// starts the game
	void init(RenderSystem* renderer, TurnBasedSystem* turn_based_arg);

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

	// handle player movement
	void handle_player_movement(int key, int action);

	void handle_menu(int key, TurnBasedSystem* turn_based);

	void handle_mini(int bpm);

	// handle option selection 
	void handle_selection();

	// get stage 
	int get_stage();
private:
	// Input callback functions
	void on_key(int key, int, int action, int mod);
	void on_mouse_move(vec2 pos);

	// restart level
	void restart_game();

	// change stage 
	void change_stage(int level);

	// OpenGL window handle
	GLFWwindow* window;

	// Number of bug eaten by the chicken, displayed in the window title
	unsigned int points;

	// Game state
	RenderSystem* renderer;
	TurnBasedSystem* turn_based;
	float current_speed;
	float player_speed;
	float next_eagle_spawn;
	float next_bug_spawn;
	float next_enemy_spawn;
	Entity player_chicken;
	int stage;

	// music references
	Mix_Music* background_music;
	Mix_Music* turn_based_music;
	Mix_Music* minigame_music;
	Mix_Chunk* change_selection_effect;
	Mix_Chunk* chicken_dead_sound;
	Mix_Chunk* chicken_eat_sound;
	Mix_Chunk* attack_sound;

	// C++ random number generator
	std::default_random_engine rng;
	std::uniform_real_distribution<float> uniform_dist; // number between 0..1
	std::uniform_real_distribution<float> wide_dist; // number between -1..1
};
