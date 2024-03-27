// Header
#include "world_system.hpp"
#include "world_init.hpp"
#include "physics_system.hpp"
#include "stage_system/stage_system.hpp"

// stlib
#include <cassert>
#include <sstream>
#include <iostream>
#include <array>
#include <chrono>

// Game configuration
const size_t MAX_EAGLES = 15;
const size_t MAX_BUG = 5;
const size_t EAGLE_DELAY_MS = 2000 * 3;
const size_t BUG_DELAY_MS = 5000 * 3;
const size_t ENEMY_DELAY_MS = 2000 * 3;

bool tutorialOn = false;
Entity tutorial;
std::chrono::steady_clock::time_point last_frame_time = std::chrono::steady_clock::now();
int frames_since_prev_second = 0;
int fps = 0;

// Create the bug world
WorldSystem::WorldSystem()
	: player_speed(300.f)
	, next_eagle_spawn(0.f)
	, next_bug_spawn(0.f)
	, next_enemy_spawn(0.f)
	, stage(0) {
	// Seeding rng with random device
	rng = std::default_random_engine(std::random_device()());
}

WorldSystem::~WorldSystem() {
	// Destroy music components
	if (main_menu_music != nullptr)
		Mix_FreeMusic(main_menu_music);
	if (background_music != nullptr)
		Mix_FreeMusic(background_music);
	if (cutscene_music != nullptr)
		Mix_FreeMusic(cutscene_music);
	if (turn_based_music != nullptr)
		Mix_FreeMusic(turn_based_music); 
	if (minigame_music != nullptr)
		Mix_FreeMusic(minigame_music); 
	if (change_selection_effect != nullptr)
		Mix_FreeChunk(change_selection_effect);
	if (chicken_dead_sound != nullptr)
		Mix_FreeChunk(chicken_dead_sound);
	if (chicken_eat_sound != nullptr)
		Mix_FreeChunk(chicken_eat_sound);
	if (attack_sound != nullptr)
		Mix_FreeChunk(attack_sound);
	Mix_CloseAudio();

	// Destroy all created components
	registry.clear_all_components();

	// Close the window
	glfwDestroyWindow(window);
}

// Debugging
namespace {
	void glfw_err_cb(int error, const char *desc) {
		fprintf(stderr, "%d: %s", error, desc);
	}
}

// World initialization
// Note, this has a lot of OpenGL specific things, could be moved to the renderer
GLFWwindow* WorldSystem::create_window() {
	///////////////////////////////////////
	// Initialize GLFW
	glfwSetErrorCallback(glfw_err_cb);
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW");
		return nullptr;
	}

	//-------------------------------------------------------------------------
	// If you are on Linux or Windows, you can change these 2 numbers to 4 and 3 and
	// enable the glDebugMessageCallback to have OpenGL catch your mistakes for you.
	// GLFW / OGL Initialization
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_RESIZABLE, 0);

	// Create the main window (for rendering, keyboard, and mouse input)
	window = glfwCreateWindow(window_width_px, window_height_px, "Chicken Game Assignment", nullptr, nullptr);
	if (window == nullptr) {
		fprintf(stderr, "Failed to glfwCreateWindow");
		return nullptr;
	}

	// Setting callbacks to member functions (that's why the redirect is needed)
	// Input is handled using GLFW, for more info see
	// http://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowUserPointer(window, this);
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_key(_0, _1, _2, _3); };
	glfwSetKeyCallback(window, key_redirect);

	//////////////////////////////////////
	// Loading music and sounds with SDL
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "Failed to initialize SDL Audio");
		return nullptr;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
		fprintf(stderr, "Failed to open audio device");
		return nullptr;
	}

	main_menu_music = Mix_LoadMUS(audio_path("main_menu_music.wav").c_str());
	background_music = Mix_LoadMUS(audio_path("music.wav").c_str());
	cutscene_music = Mix_LoadMUS(audio_path("cutscene_music.wav").c_str());
	turn_based_music = Mix_LoadMUS(audio_path("turn_based.wav").c_str());
	minigame_music = Mix_LoadMUS(audio_path("120bpmwithmetronome.wav").c_str());
	//minigame_music = Mix_LoadMUS(audio_path("metronome.wav").c_str());
	change_selection_effect = Mix_LoadWAV(audio_path("change_selection_effect.wav").c_str());
	chicken_dead_sound = Mix_LoadWAV(audio_path("chicken_dead.wav").c_str());
	chicken_eat_sound = Mix_LoadWAV(audio_path("chicken_eat.wav").c_str());
	attack_sound = Mix_LoadWAV(audio_path("attack.wav").c_str());

	if (background_music == nullptr || turn_based_music == nullptr || minigame_music == nullptr || 
			chicken_dead_sound == nullptr || chicken_eat_sound == nullptr) {
		fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
			audio_path("music.wav").c_str(),
			audio_path("turn_based.wav").c_str(),
			audio_path("120bpmwithmetronome.wav").c_str(),
			// audio_path("metronome.wav").c_str(),
			audio_path("chicken_dead.wav").c_str(),
			audio_path("chicken_eat.wav").c_str(), 
			audio_path("attack.wav").c_str());
		return nullptr;
	}

	return window;
}

void WorldSystem::init(RenderSystem* renderer_arg, TurnBasedSystem* turn_based_arg, StageSystem* stage_system_arg,
	MainMenuSystem* main_menu_system_arg, OverworldSystem* overworld_system_arg, CutSceneSystem* cutscene_system_arg,
	CombatSystem* combat_system_arg, MinigameSystem* minigame_system_arg) {
	// Global Systems Set up
	this->renderer = renderer_arg;
	this->turn_based = turn_based_arg;
	this->stage_system = stage_system_arg;
	stage = 0;

	this->main_menu_system = main_menu_system_arg;
	this->overworld_system = overworld_system_arg;
	this->cutscene_system = cutscene_system_arg;
	this->combat_system = combat_system_arg;
	this->minigame_system = minigame_system_arg;

	// Playing background music indefinitely
	Mix_PlayMusic(main_menu_music, -1);
	fprintf(stderr, "Loaded music\n");

	// Setting up the stage to music map
	stage_music_map[StageSystem::Stage::main_menu] = main_menu_music;
	stage_music_map[StageSystem::Stage::overworld] = background_music;
	stage_music_map[StageSystem::Stage::cutscene] = cutscene_music;
	stage_music_map[StageSystem::Stage::turn_based] = turn_based_music;
	stage_music_map[StageSystem::Stage::minigame] = minigame_music;

	// Set all states to default
    restart_game();
}

void WorldSystem::set_fps(float elapsed_ms_since_last_update) {
	// Updating window title with fps
	std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsed_seconds = current_time - last_frame_time;
	frames_since_prev_second++;

	if (elapsed_seconds.count() >= 0.5f) {
		fps = frames_since_prev_second;
		frames_since_prev_second = 0;
		last_frame_time = current_time;
	}

	std::stringstream title_ss;

	title_ss << "FPS: " << fps << " | ";

	glfwSetWindowTitle(window, title_ss.str().c_str());
}

void WorldSystem::set_music(StageSystem::Stage curr_stage) {
	if (stage_system->get_music_changed()) {
		Mix_PlayMusic(stage_music_map[curr_stage], -1);
		Mix_VolumeMusic(MIX_MAX_VOLUME * 0.50);
		stage_system->set_music_changed();
	}
}

// Update our game world
bool WorldSystem::step(float elapsed_ms_since_last_update) {
	set_fps(elapsed_ms_since_last_update);

	StageSystem::Stage curr_stage = stage_system->get_current_stage();
	// Removing out of screen entities
	auto& motions_registry = registry.motions;

	set_music(curr_stage);

	// Remove debug info from the last step
	while (registry.debugComponents.entities.size() > 0)
	    registry.remove_all_components_of(registry.debugComponents.entities.back());

	// Handle overworld stepping
	next_enemy_spawn -= elapsed_ms_since_last_update * current_speed;
	if (curr_stage == StageSystem::Stage::overworld) {
		// Remove entities that leave the screen on the left side
		// Iterate backwards to be able to remove without unterfering with the next object to visit
		// (the containers exchange the last element with the current)
		for (int i = (int)motions_registry.components.size() - 1; i >= 0; --i) {
			Motion& motion = motions_registry.components[i];
			if (motion.position.x + abs(motion.scale.x) < 0.f) {
				if (!registry.players.has(motions_registry.entities[i]) &&
					!registry.backgrounds.has(motions_registry.entities[i])) // don't remove the player or background
					registry.remove_all_components_of(motions_registry.entities[i]);
			}
		}

		if (registry.enemyDrinks.components.size() <= MAX_EAGLES && next_enemy_spawn < 0.f) {
			// Reset timer
			next_enemy_spawn = (ENEMY_DELAY_MS / 2) + uniform_dist(rng) * (ENEMY_DELAY_MS / 2);
			// Create enemy drink with random initial velocity, position
			Entity test = createEnemyDrink(renderer,
				// TODO: make negative velocity possible
				vec2((uniform_dist(rng) - 0.5) * 200.f, (uniform_dist(rng) - 0.5) * 200.f),
				// TODO: fix to spawn from only the edges
				vec2(uniform_dist(rng) * (window_width_px - 100.f), BG_HEIGHT + uniform_dist(rng) * (window_height_px - BG_HEIGHT)));
		}

		// Processing the chicken state
		assert(registry.screenStates.components.size() <= 1);
		ScreenState& screen = registry.screenStates.components[0];

	// process attacks
	float min_attack_counter_ms = 700.f;
	for (Entity entity : registry.attackTimers.entities) {
		// progress timer
		AttackTimer& counter = registry.attackTimers.get(entity);
		counter.counter_ms -= elapsed_ms_since_last_update;
		if (counter.counter_ms < min_attack_counter_ms) {
			min_attack_counter_ms = counter.counter_ms;
		}

		// stop attack once timer expires
		if (counter.counter_ms < 0) {
			registry.attackTimers.remove(entity);
			return true;
		}
	}

	// deal with injury graphics
	// i do not know why this does not work if its in turn based system
	float min_injury_counter_ms = 3000.f;
	for (Entity entity : registry.injuryTimers.entities) {
		// progress timer, 
		InjuredTimer& counter = registry.injuryTimers.get(entity);
		counter.counter_ms -= elapsed_ms_since_last_update;
		//std::cout << "curr time: " << (3000.f - counter.counter_ms) / 3000.f << '\n';
		counter.redness_factor -= 1.f / 200.f; 
		if (counter.counter_ms < min_injury_counter_ms) {
			min_injury_counter_ms = counter.counter_ms;
		}

		// stop shaking the attacked character after 3 seconds
		if (counter.counter_ms < 0) {
			//std::cout << "end time: " << (3000.f - counter.counter_ms) / 3000.f << '\n';
			registry.injuryTimers.remove(entity);
		}
	}


	}

	// Handle turn based stepping
	if (curr_stage == StageSystem::Stage::turn_based) {
		if (out_of_combat) {
			combat_system->handle_level(renderer);
		}
		else {
			combat_system->handle_turn_rendering();
		}
	}

	//// Countdown minigame for ending
	//if (stage == 2) {
	//	bool ended = false;
	//	float minigame_timer_counter_ms = 10000.f;
	//	for (Entity entity : registry.miniGameTimer.entities) {
	//		// progress timer
	//		MiniGameTimer& counter = registry.miniGameTimer.get(entity);
	//		counter.counter_ms -= elapsed_ms_since_last_update;
	//		if (counter.counter_ms < minigame_timer_counter_ms) {
	//			minigame_timer_counter_ms = counter.counter_ms;
	//		}

	//		if (counter.inter_state) {
	//			counter.inter_timer -= elapsed_ms_since_last_update;

	//			if (counter.inter_timer < 0) {
	//				registry.renderRequests.remove(entity);
	//				registry.renderRequests.insert(
	//					entity,
	//					{ TEXTURE_ASSET_ID::MINIGAMECUP,
	//					EFFECT_ASSET_ID::TEXTURED,
	//					GEOMETRY_BUFFER_ID::SPRITE }
	//				);

	//				counter.inter_state = false;
	//				counter.inter_timer = 500.f;
	//			}
	//		}

	//		// stop minigame once timer expires
	//		if (counter.counter_ms < 0) {
	//			// Change turn-based values here to deal damage based on score
	//			Minigame& minigame = registry.miniGame.get(entity);
	//			std::cout << "SCORE: " << minigame.score << '\n';

	//			registry.remove_all_components_of(entity);
	//			ended = true;

	//			// Assign score to ability
	//			Entity active_char_entity = turn_based->get_active_character();
	//			// We need an ability rather than "Basic Attack" in order to multiply the ability's power by minigame.score
	//			handle_attack(active_char_entity, "Basic Attack");
	//		}
	//	}

	//	for (Entity entity : registry.miniGameResTimer.entities) {
	//		if (ended) {
	//			registry.remove_all_components_of(entity);
	//			change_stage(1);
	//			break;
	//		}
	//
	//		if (registry.renderRequests.has(entity)) {
	//			MiniGameResTimer& counter = registry.miniGameResTimer.get(entity);
	//			counter.counter_ms -= elapsed_ms_since_last_update;

	//			if (counter.counter_ms < 0) {
	//				registry.renderRequests.remove(entity);
	//				counter.counter_ms = 250.f;
	//			}
	//		}
	//	}
	//}

	return true;
}

// Reset the world state to its initial state
void WorldSystem::restart_game() {
	stage_system->set_stage(StageSystem::Stage::main_menu);

	// Debugging for memory/component leaks
	registry.list_all_components();
	printf("Restarting\n");

	// Set Default values for systems
	main_menu_system->init(stage_system);
	overworld_system->init(stage_system);
	cutscene_system->init(stage_system);
	combat_system->init(stage_system, turn_based);
	minigame_system->init(stage_system);

	// Reset the game speed
	current_speed = 1.f;

	// Reset the stage
	stage = 0;

	// Remove all entities that we created
	// All that have a motion, we could also iterate over all bug, eagles, ... but that would be more cumbersome
	while (registry.motions.entities.size() > 0)
		registry.remove_all_components_of(registry.motions.entities.back());

	while (registry.menu.entities.size() > 0) 
		registry.remove_all_components_of(registry.menu.entities.back());

	// Debugging for memory/component leaks
	registry.list_all_components();

	// create new background
	createBackgroundScroller(renderer, { window_width_px / 2, BG_HEIGHT / 2 });
	
	// create new foreground (with lights)
	createForegroundScroller(renderer, { window_width_px / 2, BG_HEIGHT + (FG_HEIGHT / 2) }, false);
	createForegroundScroller(renderer, { window_width_px / 2, BG_HEIGHT / 2 }, true);

	// Create a new chicken
	player_chicken = createChicken(renderer, { window_width_px / 2, window_height_px - 200 });
	registry.colors.insert(player_chicken, { 1, 0.8f, 0.8f });

	// Create Background for turn based battle
	createBackgroundBattle(renderer, { window_width_px / 2.0, window_height_px / 2.0 });

	// Create Background for cutscenes
	createBackgroundCutscene(renderer, { window_width_px / 2.0, window_height_px / 2.0 });

	// Create the main menu
	createMainMenu(renderer, { 0, 0 });

	// Create tutorials
	createTutorialWindow(renderer, vec2(window_width_px / 2, window_height_px / 2), 1);
	createTutorialWindow(renderer, vec2(window_width_px / 2, window_height_px / 2), 2);
}

// Compute collisions between entities
void WorldSystem::handle_collisions() {
	// Loop over all collisions detected by the physics system
	auto& collisionsRegistry = registry.collisions;
	for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
		// The entity and its collider
		Entity entity = collisionsRegistry.entities[i];
		Entity entity_other = collisionsRegistry.components[i].other;

		// For now, we are only interested in collisions that involve the chicken
		if (registry.players.has(entity)) {

			// Checking Player - Attack collisions
			if (registry.enemyDrinks.has(entity_other)) {
				// initiate fight if player is attacking
				if (registry.attackTimers.has(entity)) {
					// Scream. we can replace this with a diff sound later
					Mix_PlayChannel(-1, chicken_dead_sound, 0);

					// potential problem: if we don't remove the enemy it might keep colliding and screaming
					registry.remove_all_components_of(entity_other);

					// We also need to kill all other eagles
					for (Entity enemies : registry.enemyDrinks.entities) {
						registry.remove_all_components_of(enemies);
					}

					// Stage = 1 maps to turn based
					stage_system->set_stage(StageSystem::Stage::cutscene);
				}
			}
		}
	}

	// Remove all collisions from this simulation step
	registry.collisions.clear();
}

// Should the game be over ?
bool WorldSystem::is_over() const {
	return bool(glfwWindowShouldClose(window));
}

// check if player is in bounds, keep it in bounds if not
// not using this anymore, but might later on so will keep it commented
//bool WorldSystem::player_in_bounds(Motion* motion, bool is_x) {
//	bool in_bounds;
//	if (is_x) {
//		float x_pos = motion->position.x;
//		bool left = x_pos >= 50.f;
//		bool right = x_pos <= (window_width_px - 50.f);
//		in_bounds = left && right;
//
//		if (!left) motion->position.x = 50.f;
//		if (!right) motion->position.x = (window_width_px - 50.f);
//
//	}
//	else {
//		float y_pos = motion->position.y;
//		bool up = y_pos >= 50.f;
//		bool down = y_pos <= (window_height_px - 50.f);
//		in_bounds = up && down;
//
//		if (!up) motion->position.y = 50.f;
//		if (!down) motion->position.y = (window_height_px - 50.f);
//	}
//
//	return in_bounds;
//}

void WorldSystem::handle_mini(int bpm) {
	bool hit = false;
	// Assuming that the bpm is based on quarter notes and it's 4/4
	for (Entity entity : registry.miniGameTimer.entities) {
		MiniGameTimer& timer = registry.miniGameTimer.get(entity); 
		Minigame& minigame = registry.miniGame.get(entity);

		std::cout << timer.counter_ms << '\n';

		// measure duration is in ms 
		int beat_duration = (60000 / bpm);
		int measure_duration = beat_duration * 4;
		float beat_error = beat_duration / 2;

		float modded = (int) timer.counter_ms % measure_duration;

		std::cout << modded << '\n';

		// trying to hit every first beat
		if (modded <= beat_error && !(timer.counter_ms < 0 + beat_error)) {
			std::cout << "YOU HIT IT early" << '\n';
		}
		else if (modded >= (measure_duration - beat_error)) {
			std::cout << "YOU HIT IT late" << '\n';
		}
		else {
			std::cout << "Not quite my tempo." << '\n';
			break;
		}

		minigame.score += 1;
		// changing cup's texture
		if (!(timer.inter_state)) {
			registry.renderRequests.remove(entity);
			registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::MINIGAMEINTER,
				EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE }
			);
			timer.inter_state = true;
		} 
		hit = true;
	}

	// Changing the result value 
	for (Entity entity : registry.miniGameResTimer.entities) {
		registry.renderRequests.remove(entity); 

		if (hit) {
			registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::MINIGAMESUCCESS,
				EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE }
			);
		} 
		else {
			registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::MINIGAMEFAIL,
				EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE }
			);
		}
	}
}

void WorldSystem::change_stage(int level) {
	if (level == 0) {
		return;
	} 
	else if (level == 1) {
		stage = 1;
		// Anything  we need to set upon changing back should go here or in step
		// Remove all minigame entities 
		for (Entity entity : registry.miniGame.entities) {
			registry.remove_all_components_of(entity);
		}
		// Replace all previous entities from render requests

		for (Entity entity : registry.motions.entities) {
			if (registry.renderRequests.has(entity)) {
				RenderRequest& render = registry.renderRequests.get(entity);
				render.shown = true;
			}
		}
		
		
	}
	else if (level == 2) {
		stage = 2;
		// Play music
		Mix_PlayMusic(minigame_music, -1);
		// Anything else we need to set upon changing to mini-game mode should go here or in step
		// Remove all previous entities from render requests
		for (Entity entity : registry.motions.entities) {
			if (registry.renderRequests.has(entity)) {
				RenderRequest& render = registry.renderRequests.get(entity);
				render.shown = false;
			}
		}
		createCup(renderer, { window_width_px / 2, window_height_px / 2 });
		createMiniResult(renderer, { window_width_px / 4, window_height_px / 2 });
	}
}

// On key callback
void WorldSystem::on_key(int key, int, int action, int mod) {
	switch (stage_system->get_current_stage()) {
	case StageSystem::Stage::main_menu:
		main_menu_system->handle_menu_keys(key, action);
		break;
	case StageSystem::Stage::overworld:
		overworld_system->handle_overworld_keys(key, action, player_speed);
		break;
	case StageSystem::Stage::cutscene:
		cutscene_system->handle_cutscene_keys(key, action);
		break;
	case StageSystem::Stage::turn_based: {
		CombatSystem::SoundMapping play_sound = combat_system->handle_turnbased_keys(key, action);
		// handle sound effects
		switch (play_sound) {
		case CombatSystem::SoundMapping::change_sound:
			Mix_PlayChannel(-1, change_selection_effect, 0);
			break;
		case CombatSystem::SoundMapping::attack_sound:
			Mix_PlayChannel(-1, attack_sound, 0);
			break;
		}

		break;
	}
	case StageSystem::Stage::minigame:
		minigame_system->handle_minigame_key(key, action);
		break;
	}

	// Minigame 
	if (action == GLFW_PRESS && key == GLFW_KEY_D) {
		if (stage == 2)
			handle_mini(120);
	}


	// Going to Main Menu manually

	if (action == GLFW_RELEASE && key == GLFW_KEY_M) {
		stage_system->set_stage(StageSystem::Stage::main_menu);
	}

	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);

        restart_game();
	}

	// Exit game 
	if (action == GLFW_RELEASE && key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
	}

	// Debugging
	if (key == GLFW_KEY_D) {
		if (action == GLFW_RELEASE && stage == 0) {
			debugging.in_debug_mode = !debugging.in_debug_mode;
		}
		else {
			debugging.in_debug_mode = false;
		}
	}

	// Control the current speed with `<` `>`
	if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_COMMA) {
		current_speed -= 0.1f;
		printf("Current speed = %f\n", current_speed);
	}
	if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_PERIOD) {
		current_speed += 0.1f;
		printf("Current speed = %f\n", current_speed);
	}
	current_speed = fmax(0.f, current_speed);
}