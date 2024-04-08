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
	if (minigame_select_music != nullptr)
		Mix_FreeMusic(minigame_select_music);
	if (minigame_practice_metronome != nullptr)
		Mix_FreeMusic(minigame_practice_metronome);
	if (minigame_music != nullptr)
		Mix_FreeMusic(minigame_music); 

	// Destroy sound effect components
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

	// load music
	main_menu_music = Mix_LoadMUS(audio_path("main_menu_music.wav").c_str());
	background_music = Mix_LoadMUS(audio_path("music.wav").c_str());
	cutscene_music = Mix_LoadMUS(audio_path("cutscene_music.wav").c_str());
	turn_based_music = Mix_LoadMUS(audio_path("turn_based.wav").c_str());
	minigame_select_music = Mix_LoadMUS(audio_path("minigame_select.wav").c_str());
	minigame_practice_metronome = Mix_LoadMUS(audio_path("metronome.wav").c_str());
	minigame_music = Mix_LoadMUS(audio_path("120bpmwithmetronome.wav").c_str());
	// load sound effects
	change_selection_effect = Mix_LoadWAV(audio_path("change_selection_effect.wav").c_str());
	chicken_dead_sound = Mix_LoadWAV(audio_path("chicken_dead.wav").c_str());
	chicken_eat_sound = Mix_LoadWAV(audio_path("chicken_eat.wav").c_str());
	attack_sound = Mix_LoadWAV(audio_path("attack.wav").c_str());

	if (background_music == nullptr || turn_based_music == nullptr || minigame_music == nullptr || 
			chicken_dead_sound == nullptr || chicken_eat_sound == nullptr) {
		fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
			audio_path("main_menu_music.wav").c_str(),
			audio_path("music.wav").c_str(),
			audio_path("cutscene_music.wav").c_str(),
			audio_path("turn_based.wav").c_str(),
			audio_path("minigame_select.wav").c_str(),
			audio_path("120bpmwithmetronome.wav").c_str(),
			audio_path("metronome.wav").c_str(),
			audio_path("change_selection_effect.wav").c_str(),
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
	stage_music_map[StageSystem::Stage::minigame] = minigame_select_music;

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

	// to pass time into overworld system.
	overworld_system->update_time(elapsed_ms_since_last_update);

	// Handle overworld stepping
	next_enemy_spawn -= elapsed_ms_since_last_update * current_speed;
	if (curr_stage == StageSystem::Stage::overworld) {

		// check the position of the player to see if it is at the next level and if so, set velocities to 0.
		// Not sure if player_chicken is being used
		/*if (registry.players.has(player_chicken)) {
			registry.players.get(player_chicken);
		}*/


		// This is tied with the addToAnimation() calls in the key press functions in OverworldSystem
		if (registry.animations.has(registry.players.entities[0])) {
			for (Entity entity_in_animation : registry.animations.entities) {
				Animation& animation_component = registry.animations.get(entity_in_animation);
				Motion& motion_component = registry.motions.get(entity_in_animation);
				
				float t = animation_component.current_ms / animation_component.total_ms;

				vec2 newpos = overworld_system->getBezierPath(animation_component.start_pos.x, animation_component.start_pos.y, animation_component.end_pos.x, animation_component.end_pos.y, t);
				// std::cout << "I AM GETTING HERE, newpos.x = " << newpos.x << std::endl;
				// std::cout << "I AM GETTING HERE, newpos.y = " << newpos.y << std::endl;

				std::cout << "THIS IS THE PATH OF BEZIER: " << std::endl;
				std::cout << newpos.x << ", " << newpos.y << std::endl;

				motion_component.position = newpos;
				
				animation_component.current_ms += elapsed_ms_since_last_update;
				


				if (animation_component.current_ms > animation_component.total_ms) {
					std::cout << "HERE ARE SOME STATS: " << std::endl;
					std::cout << "I came from this level: " << overworld_system->prev_node.level_number << std::endl;

					std::cout << "The position of this previous thing is: " << "(" << overworld_system->prev_node.position.x << ", " << overworld_system->prev_node.position.y << ")" << std::endl;
					//std::cout << " " << << std::endl;

					overworld_system->nearest_node.position = motion_component.position;

					std::cout << "I am now at this level: " << overworld_system->nearest_node.level_number << std::endl;
					std::cout << "The position of this current thing is: " << "(" << overworld_system->nearest_node.position.x << ", " << overworld_system->nearest_node.position.y << ")" << std::endl;
					//std::cout << " " << << std::endl;

					overworld_system->updatePlayerVelocityTowardsTarget(elapsed_ms_since_last_update);

					registry.animations.remove(entity_in_animation);
				}
			}
		}
		
		
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

		if (registry.levelNode.size() == 0) {
			create_overworld_levels(5);
		}
		
		// This helper is replacing the block that spawns in enemies. 


		// This block spawns in enemies
		// 
		//if (registry.enemyDrinks.components.size() <= MAX_EAGLES && next_enemy_spawn < 0.f) {
		//	// Reset timer
		//	next_enemy_spawn = (ENEMY_DELAY_MS / 2) + uniform_dist(rng) * (ENEMY_DELAY_MS / 2);
		//	// Create enemy drink with random initial velocity, position
		//	Entity test = createEnemyDrink(renderer,
		//		// TODO: make negative velocity possible
		//		vec2((uniform_dist(rng) - 0.5) * 200.f, (uniform_dist(rng) - 0.5) * 200.f),
		//		// TODO: fix to spawn from only the edges
		//		vec2(uniform_dist(rng) * (window_width_px - 100.f), BG_HEIGHT + uniform_dist(rng) * (window_height_px - BG_HEIGHT)));
		//}

		//if (registry.levelNode.components.size() <= MAX_EAGLES && next_enemy_spawn < 0.f) {
		//	// Reset timer
		//	next_enemy_spawn = (ENEMY_DELAY_MS / 2) + uniform_dist(rng) * (ENEMY_DELAY_MS / 2);
		//	// Create enemy drink with random initial velocity, position
		//	Entity test = createLevelNode(renderer, LevelNode(), LevelNode(), vec2(uniform_dist(rng) * (window_width_px - 100.f), BG_HEIGHT + uniform_dist(rng) * (window_height_px - BG_HEIGHT)));
		//	}




		// Processing the chicken state
		assert(registry.screenStates.components.size() <= 1);
		ScreenState& screen = registry.screenStates.components[0];

	}

	// Handle turn based stepping
	if (curr_stage == StageSystem::Stage::turn_based) {
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

		if (out_of_combat) {

			if (registry.turnBasedEnemies.size() >= 1) {
				combat_system->handle_combat_over();
			}
			else {
				combat_system->set_selected_level(stage_system->get_current_level());
				combat_system->handle_level(renderer);
			}

			
		}
		else {
			combat_system->handle_turn_rendering();
			
			if (minigame_system->loaded) {
				// trigger attack?
				Entity charData = turn_based->get_active_character();
				int current_score = minigame_system->get_score();
				combat_system->handle_minigame_attack(charData, current_score);

				minigame_system->loaded = false;
			}
		}
	}

	// Handle cutscene stepping
	if (curr_stage == StageSystem::Stage::cutscene) {
		cutscene_system->handle_cutscene_render(renderer);

	}

	// Handle minigame stepping
	if (curr_stage == StageSystem::Stage::minigame) {
		if (minigame_system->get_not_started()) {
			if (!(minigame_system->initialized)) {
				minigame_system->handle_set_rhythm();
				minigame_system->reset_values(true);
				minigame_system->initialized = true;
			}
		}
		else {
			if (minigame_system->get_practice()) {
				// playing practice "music"
				if (!(minigame_system->practice_music_start)) {
					Mix_PlayMusic(minigame_practice_metronome, -1);
					Mix_VolumeMusic(MIX_MAX_VOLUME * 0.50);
					minigame_system->practice_music_start = true;
				}
			}
			else {
				// playing minigame "music"
				if (!(minigame_system->minigame_music_start)) {
					Mix_PlayMusic(minigame_music, -1);
					Mix_VolumeMusic(MIX_MAX_VOLUME * 0.50);
					minigame_system->minigame_music_start = true;
				}
			}
			minigame_system->time_since_last_press += elapsed_ms_since_last_update;
			minigame_system->minigame_overall_timer += elapsed_ms_since_last_update;

			minigame_system->minigame_step(elapsed_ms_since_last_update);
		}
	}
  
	// Handle main menu feedback timer
	if (curr_stage == StageSystem::Stage::main_menu) {
		float min_persistence_counter_ms = 1000.f;
		for (Entity entity : registry.persistanceFeedbackTimer.entities) {
			// progress timer
			PersistenceFeedbackTimer& counter = registry.persistanceFeedbackTimer.get(entity);
			counter.counter_ms -= elapsed_ms_since_last_update;
			if (counter.counter_ms < min_persistence_counter_ms) {
				min_persistence_counter_ms = counter.counter_ms;
			}

			// stop attack once timer expires
			if (counter.counter_ms < 0) {
				if (registry.textRenderRequests.has(entity)) {
					registry.textRenderRequests.remove(entity);
				}
				registry.persistanceFeedbackTimer.remove(entity);
			}
		}
	}

	return true;
}
void WorldSystem::create_overworld_levels(int num_levels) {
	// For now TODO: HELP
	if (registry.levelNode.components.size() <= num_levels) {
		// replace createEnemyDrink with createLevelNode
		
		for (int i = 0; i < num_levels; i++) {
			// 1500 is window_width_px
			vec2 levelpos = vec2((1500/num_levels)/2 + (1500/(num_levels) * i), 400 + (300*((1+i)%2)));

			std::cout << "HERE IS THE COORDINATE OF LEVEL: " << (i + 1) << std::endl;
			std::cout << levelpos.x << ", " << levelpos.y << std::endl;


			Entity level = createLevelNode(renderer, i+1, levelpos);
			
			
			// TODO // set the first levelnode to 3 just for testing, DELETE AND FIX THIS LATERRRRR:
			if (i == 2) {
				std::cout << "This should only run once: " << "i = 0" << std::endl;
				overworld_system->prev_node = registry.levelNode.get(level);
			}
			
		}
		
	}
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
	cutscene_system->init(stage_system, renderer);
	combat_system->init(stage_system, turn_based);
	minigame_system->init(stage_system, renderer);

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
	//createBackgroundCutscene(renderer, { window_width_px / 2.0, window_height_px / 2.0 });

	// Create the main menu
	createMainMenu(renderer, { window_width_px / 2.0, 150 });

	// Create tutorials
	createTutorialWindow(renderer, vec2(window_width_px / 2, window_height_px / 2), 1);
	createTutorialWindow(renderer, vec2(window_width_px / 2, window_height_px / 2), 2);

	// TODO: MAKE IT SO THAT THE GAME SAVES THE LAST LEVEL THAT THE PLAYER WAS ON
	if (registry.motions.has(registry.players.entities[0])) {
		Motion& player_motion = registry.motions.get(registry.players.entities[0]);
		// std::cout << "THIS IS RUN" << std::endl;
	}
	//if (registry.levelNode.has(registry.levelNode.entities[0])) {
	//	std::cout << "THESE ARE THE POSITION OF THE FIRST LEVELNODE" << std::endl;
	//	std::cout << registry.motions.get(registry.levelNode.entities[0]).position.x << std::endl;
	//	std::cout << registry.motions.get(registry.levelNode.entities[0]).position.y << std::endl;
	//	//player_motion.position = registry.motions.get(registry.levelNode.entities[0]).position;
	//}
}

// Compute collisions between entities
void WorldSystem::handle_collisions() {
	// Loop over all collisions detected by the physics system
	//auto& collisionsRegistry = registry.collisions;
	//for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
	//	// The entity and its collider
	//	Entity entity = collisionsRegistry.entities[i];
	//	Entity entity_other = collisionsRegistry.components[i].other;

	//	// For now, we are only interested in collisions that involve the chicken
	//	if (registry.players.has(entity)) {

	//		// Checking Player - Attack collisions
	//		if (registry.enemyDrinks.has(entity_other)) {
	//			// initiate fight if player is attacking
	//			if (registry.attackTimers.has(entity)) {
	//				// Scream. we can replace this with a diff sound later
	//				Mix_PlayChannel(-1, chicken_dead_sound, 0);

	//				// potential problem: if we don't remove the enemy it might keep colliding and screaming
	//				registry.remove_all_components_of(entity_other);

	//				// We also need to kill all other eagles
	//				for (Entity enemies : registry.enemyDrinks.entities) {
	//					registry.remove_all_components_of(enemies);
	//				}

	//				// Stage = 1 maps to turn based
	//				stage_system->set_stage(StageSystem::Stage::cutscene, registry.players.components[0].level_num);
	//			}
	//		}
	//	}
	//}

	// Remove all collisions from this simulation step
	registry.collisions.clear();
}

void WorldSystem::handle_level_collisions() {
	// Loop over all collisions detected by the physics system
	auto& collisionsRegistry = registry.collisions;
	for (uint i = 0; i < collisionsRegistry.components.size(); i++) {
		// The entity and its collider
		
		Entity entity = collisionsRegistry.entities[i];
		Entity entity_other = collisionsRegistry.components[i].other;

		// For now, we are only interested in collisions that involve the chicken
		if (registry.players.has(entity)) {

			// Checking Player - Attack collisions
			if (registry.levelNode.has(entity_other)) {
				// initiate fight if player is attacking
				if (registry.attackTimers.has(entity)) {
					// Scream. we can replace this with a diff sound later
					Mix_PlayChannel(-1, chicken_dead_sound, 0);

					// potential problem: if we don't remove the enemy it might keep colliding and screaming
					registry.remove_all_components_of(entity_other);

					// We also need to kill all other eagles
					for (Entity enemies : registry.levelNode.entities) {
						registry.remove_all_components_of(enemies);
					}

					// TODO: SHOULD MAP TO DIFFERENT LEVELS
					// Maybe I should just get this registry.players.components[0].level_num in the turn_based_system
					std::cout << "THIS IS THE LEVEL NUM: " << registry.players.components[0].level_num << std::endl;

					// stage_system->set_stage(StageSystem::Stage::cutscene, registry.players.components[0].level_num);
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

	// Going to Main Menu manually
	if (action == GLFW_RELEASE && key == GLFW_KEY_M) {
		minigame_system->reset_values(true);
		minigame_system->initialized = false;
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