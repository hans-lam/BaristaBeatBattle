// Header
#include "world_system.hpp"
#include "world_init.hpp"

// stlib
#include <cassert>
#include <sstream>
#include <iostream>
#include <array>
#include "physics_system.hpp"
#include "turn_based_system/character_factory/character_factory.hpp"

// Game configuration
const size_t MAX_EAGLES = 15;
const size_t MAX_BUG = 5;
const size_t EAGLE_DELAY_MS = 2000 * 3;
const size_t BUG_DELAY_MS = 5000 * 3;
const size_t ENEMY_DELAY_MS = 2000 * 3;

// Create the bug world
WorldSystem::WorldSystem()
	: points(0)
	, player_speed(300.f)
	, next_eagle_spawn(0.f)
	, next_bug_spawn(0.f)
	, next_enemy_spawn(0.f)
	, stage(0) {
	// Seeding rng with random device
	rng = std::default_random_engine(std::random_device()());
}

WorldSystem::~WorldSystem() {
	// Destroy music components
	if (background_music != nullptr)
		Mix_FreeMusic(background_music);
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
	auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_mouse_move({ _0, _1 }); };
	glfwSetKeyCallback(window, key_redirect);
	glfwSetCursorPosCallback(window, cursor_pos_redirect);

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

	background_music = Mix_LoadMUS(audio_path("music.wav").c_str());
	turn_based_music = Mix_LoadMUS(audio_path("turn_based.wav").c_str()); 
	minigame_music = Mix_LoadMUS(audio_path("metronome.wav").c_str());
	change_selection_effect = Mix_LoadWAV(audio_path("change_selection_effect.wav").c_str());
	chicken_dead_sound = Mix_LoadWAV(audio_path("chicken_dead.wav").c_str());
	chicken_eat_sound = Mix_LoadWAV(audio_path("chicken_eat.wav").c_str());
	attack_sound = Mix_LoadWAV(audio_path("attack.wav").c_str());

	if (background_music == nullptr || turn_based_music == nullptr || minigame_music == nullptr || 
			chicken_dead_sound == nullptr || chicken_eat_sound == nullptr) {
		fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
			audio_path("music.wav").c_str(),
			audio_path("turn_based.wav").c_str(),
			audio_path("metronome.wav").c_str(),
			audio_path("chicken_dead.wav").c_str(),
			audio_path("chicken_eat.wav").c_str(), 
			audio_path("attack.wav").c_str());
		return nullptr;
	}

	return window;
}

void WorldSystem::init(RenderSystem* renderer_arg, TurnBasedSystem* turn_based_arg) {
	this->renderer = renderer_arg;
	this->turn_based = turn_based_arg;
	// Playing background music indefinitely
	Mix_PlayMusic(background_music, -1);
	fprintf(stderr, "Loaded music\n");

	// Set all states to default
    restart_game();
}

// Update our game world
bool WorldSystem::step(float elapsed_ms_since_last_update) {
	// Updating window title with points
	std::stringstream title_ss;
	if (stage == 0)
		title_ss << "OVERWORLD: Hit A to attack";
	else if (stage == 1)
		title_ss << "TURN-BASED: Hit S to start turn-based, SPACE to select option, M to start minigame";
	else if (stage == 2)
		title_ss << "MINIGAME: Hit M again to go back to turn-based";
	glfwSetWindowTitle(window, title_ss.str().c_str());

	// Remove debug info from the last step
	while (registry.debugComponents.entities.size() > 0)
	    registry.remove_all_components_of(registry.debugComponents.entities.back());

	// Removing out of screen entities
	auto& motions_registry = registry.motions;

	// Remove entities that leave the screen on the left side
	// Iterate backwards to be able to remove without unterfering with the next object to visit
	// (the containers exchange the last element with the current)
	for (int i = (int)motions_registry.components.size()-1; i>=0; --i) {
	    Motion& motion = motions_registry.components[i];
		if (motion.position.x + abs(motion.scale.x) < 0.f) {
			if(!registry.players.has(motions_registry.entities[i]) && !registry.backgrounds.has(motions_registry.entities[i])) // don't remove the player or background
				registry.remove_all_components_of(motions_registry.entities[i]);
		}
	}

	// Spawning new enemy drinks
	next_enemy_spawn -= elapsed_ms_since_last_update * current_speed;
	if (stage == 0) {
		if (registry.enemyDrinks.components.size() <= MAX_EAGLES && next_enemy_spawn < 0.f) {
			// Reset timer
			next_enemy_spawn = (ENEMY_DELAY_MS / 2) + uniform_dist(rng) * (ENEMY_DELAY_MS / 2);
			// Create enemy drink with random initial velocity, position
			createEnemyDrink(renderer,
				// TODO: make negative velocity possible
				vec2((uniform_dist(rng) - 0.5) * 200.f, (uniform_dist(rng) - 0.5) * 200.f),
				// TODO: fix to spawn from only the edges
				vec2(uniform_dist(rng) * (window_width_px - 100.f), BG_HEIGHT + uniform_dist(rng) * (window_height_px - BG_HEIGHT)));
		}
	}

	// Highlighting current char 
	if (stage == 1) {

		Entity active_char_entity = turn_based->get_active_character();
		
		if (active_char_entity != emptyEntity) {


			Character* active_char = registry.characterDatas.get(active_char_entity).characterData;

			// setting player color 
			for (Entity entity : registry.partyMembers.entities) {
				vec3& color = registry.colors.get(entity);
				
				if (entity == active_char_entity) {
					// change current player character to red
					color = { 1.0f, 0.f, 0.f };
				}
				else {
					// return player character color to normal
					color = { 1, 0.8f, 0.8f };
				}
			}

			// Setting menu's active 
			for (Entity menu_entity : registry.menu.entities) {
				Menu& menu = registry.menu.get(menu_entity);
				Entity attack = menu.options[0];
				Entity rest = menu.options[1];
				MenuOption& atk = registry.menuOptions.get(attack);
				MenuOption& rst = registry.menuOptions.get(rest);

				if (menu.associated_character == active_char_entity) {
					if (!registry.renderRequests.has(attack)) {
						registry.renderRequests.insert(
							attack,
							{ TEXTURE_ASSET_ID::ATTACKBUTTON, // TEXTURE_COUNT indicates that no txture is needed
							EFFECT_ASSET_ID::TEXTURED,
							GEOMETRY_BUFFER_ID::SPRITE });
					}
					if (!registry.renderRequests.has(rest)) {
						registry.renderRequests.insert(
							rest,
							{ TEXTURE_ASSET_ID::ITEMBUTTON, // TEXTURE_COUNT indicates that no txture is needed
							EFFECT_ASSET_ID::TEXTURED,
							GEOMETRY_BUFFER_ID::SPRITE });
					}
				}
				else {
					if (registry.renderRequests.has(attack)) {
						registry.renderRequests.remove(attack);
					}
					if (registry.renderRequests.has(rest)) {
						registry.renderRequests.remove(rest);
					}
				}

				// handle color change of option 
				for (Entity entity : menu.options) {
					vec3& color = registry.colors.get(entity);

					if (entity == menu.activeOption) {
						color.x = 0.8f;
					}
					else {
						color.x = 1;
					}
				}
			}
		}
	}

	// Processing the chicken state
	assert(registry.screenStates.components.size() <= 1);
    ScreenState &screen = registry.screenStates.components[0];

    float min_counter_ms = 3000.f;
	for (Entity entity : registry.deathTimers.entities) {
		// progress timer
		DeathTimer& counter = registry.deathTimers.get(entity);
		counter.counter_ms -= elapsed_ms_since_last_update;
		if(counter.counter_ms < min_counter_ms){
		    min_counter_ms = counter.counter_ms;
		}

		// restart the game once the death timer expired
		if (counter.counter_ms < 0) {
			registry.deathTimers.remove(entity);
			screen.darken_screen_factor = 0;
            restart_game();
			return true;
		}
	}
	// reduce window brightness if any of the present chickens is dying
	screen.darken_screen_factor = 1 - min_counter_ms / 3000;

	// process attacks
	float min_attack_counter_ms = 700.f;
	for (Entity entity : registry.attackTimers.entities) {
		// progress timer
		AttackTimer& counter = registry.attackTimers.get(entity);
		counter.counter_ms -= elapsed_ms_since_last_update;
		if (counter.counter_ms < min_attack_counter_ms) {
			min_attack_counter_ms = counter.counter_ms;
		}

		// handle attack
		Motion& player_motion = registry.motions.get(entity);
		player_motion.angle += M_PI / 12.0f;

		// stop attack once timer expires
		if (counter.counter_ms < 0) {
			registry.attackTimers.remove(entity);
			player_motion.angle = 0.f;
			return true;
		}
	}

	// Countdown minigame for ending
	if (stage == 2) {
		bool ended = false;
		float minigame_timer_counter_ms = 10000.f;
		for (Entity entity : registry.miniGameTimer.entities) {
			// progress timer 
			MiniGameTimer& counter = registry.miniGameTimer.get(entity);
			counter.counter_ms -= elapsed_ms_since_last_update;
			if (counter.counter_ms < minigame_timer_counter_ms) {
				minigame_timer_counter_ms = counter.counter_ms;
			}

			if (counter.inter_state) {
				counter.inter_timer -= elapsed_ms_since_last_update;

				if (counter.inter_timer < 0) {
					registry.renderRequests.remove(entity); 
					registry.renderRequests.insert(
						entity,
						{ TEXTURE_ASSET_ID::MINIGAMECUP,
						EFFECT_ASSET_ID::TEXTURED,
						GEOMETRY_BUFFER_ID::SPRITE }
					);

					counter.inter_state = false;
					counter.inter_timer = 500.f;
				}
			}

			// stop minigame once timer expires 
			if (counter.counter_ms < 0) {
				// Change turn-based values here to deal damage based on score
				Minigame& minigame = registry.miniGame.get(entity); 
				std::cout << "SCORE: " << minigame.score << '\n';

				registry.remove_all_components_of(entity);
				ended = true;

				// Assign score to ability 
				Entity active_char_entity = turn_based->get_active_character(); 
				// We need an ability rather than "Basic Attack" in order to multiply the ability's power by minigame.score
				handle_attack(active_char_entity, "Basic Attack");
			}
		}

		for (Entity entity : registry.miniGameResTimer.entities) {
			if (ended) {
				registry.remove_all_components_of(entity);
				change_stage(1);
				break;
			}
			
			if (registry.renderRequests.has(entity)) {
				MiniGameResTimer& counter = registry.miniGameResTimer.get(entity);
				counter.counter_ms -= elapsed_ms_since_last_update;

				if (counter.counter_ms < 0) {
					registry.renderRequests.remove(entity);
					counter.counter_ms = 250.f;
				}
			}
		}
	}

	return true;
}

// Reset the world state to its initial state
void WorldSystem::restart_game() {
	// Debugging for memory/component leaks
	registry.list_all_components();
	printf("Restarting\n");

	// Reset the game speed
	current_speed = 1.f;

	// Reset the stage
	stage = 0;

	// Remove all entities that we created
	// All that have a motion, we could also iterate over all bug, eagles, ... but that would be more cumbersome
	while (registry.motions.entities.size() > 0)
		registry.remove_all_components_of(registry.motions.entities.back());

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

					// remove all overworld bgs
					for (uint i = 0; i < registry.backgrounds.entities.size(); i++) {
						registry.remove_all_components_of(registry.backgrounds.entities[i]);
					}

					for (Entity foregrounds : registry.foregrounds.entities) {
						registry.remove_all_components_of(foregrounds);
					}

					// We also need to kill all other eagles
					for (Entity enemies : registry.enemyDrinks.entities) {
						registry.remove_all_components_of(enemies);
					}

					// Stage = 1 maps to turn based
					stage = 1;
					// Delete current player
					registry.remove_all_components_of(entity);

					// Play music
					Mix_PlayMusic(turn_based_music, -1);
					// max volume is at 128; this sets it to 75%
					Mix_VolumeMusic(MIX_MAX_VOLUME - 32);
				}
			}
			//// Checking Player - Eatable collisions
			//else if (registry.eatables.has(entity_other)) {
			//	if (!registry.deathTimers.has(entity)) {
			//		// chew, count points, and set the LightUp timer
			//		registry.remove_all_components_of(entity_other);
			//		Mix_PlayChannel(-1, chicken_eat_sound, 0);
			//		++points;

			//		// !!! TODO A1: create a new struct called LightUp in components.hpp and add an instance to the chicken entity by modifying the ECS registry
			//	}
			//}
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

// chicken movement helper
void WorldSystem::handle_player_movement(int key, int action) {

	for (uint i = 0; i < registry.players.size(); i++) {
		Motion& player_motion = registry.motions.get(registry.players.entities[i]);
		// do not move player if it is dying
		if (registry.deathTimers.has(registry.players.entities[i])) continue;
		// check if player is in bounds
		bool is_x = (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT);
		//if (!player_in_bounds(&player_motion, is_x)) {
		//	action = GLFW_RELEASE;
		//}

		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			if (key == GLFW_KEY_UP) {
				player_motion.velocity.y = -player_speed;
			}
			else if (key == GLFW_KEY_LEFT) {
				player_motion.velocity.x = -player_speed;
			}
			else if (key == GLFW_KEY_DOWN) {
				player_motion.velocity.y = player_speed;
			}
			else if (key == GLFW_KEY_RIGHT) {
				player_motion.velocity.x = player_speed;
			}

		}
		else if (action == GLFW_RELEASE) {
			if (key == GLFW_KEY_UP) {
				player_motion.velocity.y = 0.0f;
			}
			else if (key == GLFW_KEY_LEFT) {
				player_motion.velocity.x = 0.0f;
			}
			else if (key == GLFW_KEY_DOWN) {
				player_motion.velocity.y = 0.0f;
			}
			else if (key == GLFW_KEY_RIGHT) {
				player_motion.velocity.x = 0.0f;
			}
		}
	}


}

void WorldSystem::handle_menu(int key, TurnBasedSystem* turn_based) {

	Entity active_char_entity = turn_based->get_active_character();

	if (active_char_entity != emptyEntity) {

		Character* active_char = registry.characterDatas.get(active_char_entity).characterData;
		
		for (Entity menu_entity : registry.menu.entities) {
			Menu& menu = registry.menu.get(menu_entity);
			int arrayLen = std::end(menu.options) - std::begin(menu.options);
			int index = -1;
			for (int i = 0; i < arrayLen; i++) {
				if (menu.activeOption == menu.options[i]) {
					index = i; 
					break;
				}
			}
		
			if (menu.associated_character == active_char_entity) {
				if (key == GLFW_KEY_UP) {
					Mix_PlayChannel(-1, change_selection_effect, 0); 
					if (index > 0) {
						menu.activeOption = menu.options[index - 1];
					}
					else {
						menu.activeOption = menu.options[(arrayLen - 1)];
					}
				}
				else if (key == GLFW_KEY_DOWN) {
					Mix_PlayChannel(-1, change_selection_effect, 0); 
					if (index < (arrayLen - 1)) {
						menu.activeOption = menu.options[index + 1];
					}
					else {
						menu.activeOption = menu.options[0];
					}
				}
				break;
			}
		}
	}
}

void WorldSystem::handle_attack(Entity active_char_entity, std::string ability) {
	Character* active_char = registry.characterDatas.get(active_char_entity).characterData;

	if (!out_of_combat) {
		Mix_PlayChannel(-1, attack_sound, 0);

		Entity enemy_target_entity = registry.turnBasedEnemies.entities[0];
		Character* enemy_target = registry.characterDatas.get(enemy_target_entity).characterData;

		turn_based->process_character_action(active_char->get_ability_by_name(ability), active_char, { enemy_target });
	}
}

void WorldSystem::handle_selection() {
	Entity active_char_entity = turn_based->get_active_character();
	if (active_char_entity != emptyEntity) {
		// Get active option
		for (Entity menu_entity : registry.menu.entities) {
			Menu& menu = registry.menu.get(menu_entity);

			// found correct menu
			if (menu.associated_character == active_char_entity) {
				Entity correctOption = menu.activeOption; 
				MenuOption& opComponent = registry.menuOptions.get(correctOption); 

				if (opComponent.option == "attack") {
					handle_attack(active_char_entity, "Basic Attack");
				}
				else if (opComponent.option == "rest") {
					// TODO USE REST ABILITY

					// set stage to 2, which is mini-game mapping
					if (stage != 2) {
						change_stage(2);
					}
				}
			}
		}
	}
}

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

void player_attack() {
	for (uint i = 0; i < registry.players.size(); i++) {
		Entity entity = registry.players.entities[i];
		if (!registry.attackTimers.has(entity)) {
			registry.attackTimers.emplace(registry.players.entities[i]);
		}
	}
}

void WorldSystem::change_stage(int level) {
	if (level == 0) {
		return;
	} 
	else if (level == 1) {
		stage = 1;
		// Play music
		Mix_PlayMusic(turn_based_music, -1);
		// max volume is at 128; this sets it to 75%
		Mix_VolumeMusic(MIX_MAX_VOLUME - 32);
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
	// handle movement
	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT || key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
		if (stage == 0) {
			handle_player_movement(key, action);
		}
		else if (stage == 1) {
			if (action == GLFW_RELEASE) {
				handle_menu(key, turn_based);
			}
		}
	}

	// player attack
	if (action == GLFW_PRESS && key == GLFW_KEY_A) {
		if (stage == 0)
			player_attack();
	}

	// turn based option
	if (action == GLFW_PRESS && key == GLFW_KEY_SPACE) {
		if (stage == 1)
			handle_selection();
	}

	// Minigame 
	if (action == GLFW_PRESS && key == GLFW_KEY_D) {
		if (stage == 2)
			handle_mini(120);
	}

	// start encounter
	if (action == GLFW_PRESS && key == GLFW_KEY_S) {
		if (stage == 1)
			if (out_of_combat) {
				

				// enemies are created and added to turnBasedEnemy ecs
				character_factory.construct_enemy(1);
				character_factory.construct_enemy(2);

				int i = 0;
				for (Entity enemy_entity : registry.turnBasedEnemies.entities) {
					vec2 velocity = { 0.f, 0.f };
					vec2 position = { window_width_px - 100, window_height_px - (i + 1) * 200 };
					Entity entity = createEnemyDrink(renderer, velocity, position, enemy_entity);
					i++;
				}

				int j = 0;
				for (Entity party_member_entity : registry.partyMembers.entities) {
					int x_base = 100;
					vec2 position = { x_base, window_height_px - (j + 1) * 200 };
					Entity entity = createChicken(renderer, position, party_member_entity);
					registry.colors.insert(entity, { 1, 0.8f, 0.8f });

					// Creating Menu entity 
					vec2 menu_pos = { x_base + 200, window_height_px - (j + 1) * 200 };
					Entity menuEnt = createMenu(renderer, menu_pos, party_member_entity);
					Menu& menu = registry.menu.get(menuEnt);

					j++;
				}

				turn_based->start_encounter();
			}

	}

	// Going to Minigame encounter manually
	if (action == GLFW_RELEASE && key == GLFW_KEY_M) {
		// set stage to 2, which is mini-game mapping
		if (stage != 2) {
			change_stage(2);
		}
		else {
			change_stage(1);
		}
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
		if (action == GLFW_RELEASE)
			debugging.in_debug_mode = false;
		else
			debugging.in_debug_mode = true;
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

void WorldSystem::on_mouse_move(vec2 mouse_position) {
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// TODO A1: HANDLE CHICKEN ROTATION HERE
	// xpos and ypos are relative to the top-left of the window, the chicken's
	// default facing direction is (1, 0)
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	(vec2)mouse_position; // dummy to avoid compiler warning
}

int WorldSystem::get_stage() {
	return stage;
}