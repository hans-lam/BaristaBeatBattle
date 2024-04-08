#include "minigame_system.hpp"
#include "world_init.hpp"

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

MinigameSystem::MinigameSystem() :
	not_started(true),
	selected_game(cool_it), 
	practice(true),
	score(0), 
	practice_music_start(false), 
	minigame_music_start(false), 
	time_since_last_press(0.0f),
	minigame_overall_timer(0.0f), 
	initialized(false), 
	loaded(false)
{}

void MinigameSystem::init(StageSystem* stage_system_arg, RenderSystem* renderer_arg) {
	stage_system = stage_system_arg; 
	renderer = renderer_arg;
	reset_values(true);

	// Minigame texts
	minigame_text_map[cool_it] = createText("Cool It!", { x_center - 400, y_center },
		1.5f, selected_color, glm::mat4(1.0f), StageSystem::Stage::minigame, true); 
	minigame_text_map[pour_it] = createText("Pour It!", { x_center, y_center },
		1.5f, not_selected_color, glm::mat4(1.0f), StageSystem::Stage::minigame, true);
	minigame_text_map[milk_it] = createText("Milk It!", { x_center + 400, y_center },
		1.5f, not_selected_color, glm::mat4(1.0f), StageSystem::Stage::minigame, true);

	// Create game selection screen's title
	minigame_select_title = createText("Choose your minigame:", { x_center - 225, y_center + 200 },
		2.0f, not_selected_color, glm::mat4(1.0f), StageSystem::Stage::minigame, true);
}

void MinigameSystem::handle_set_rhythm() {
	// Code to set rhythm randomly; this will be implemented with more rhythms
	/*srand(time(0));

	int rand_index = rand() % 4;
	curr_rhythm = &rhythms_arr[rand_index];*/

	// For now, rhythm will just be r1
	curr_rhythm = &rhythms_arr[0];
	beat_duration = (60000 / curr_rhythm->BPM); 
	// beat error should be 1 sixteenth note
	beat_error = beat_duration / 4;
	// measure duration for calculating if they hit on time or not
	measure_duration = beat_duration * 4;
}

void MinigameSystem::handle_game_selection() {
	for (int i = cool_it; i <= milk_it; i++) {
		Minigame minigame_option = static_cast<Minigame>(i);
		TextRenderRequest& trr = registry.textRenderRequests.get(minigame_text_map[minigame_option]);
		trr.shown = false;
	}
	// make title not shown too 
	registry.textRenderRequests.get(minigame_select_title).shown = false;

	// load entities according to chosen game
	switch (selected_game) {
	case cool_it:
		load_cool_it();
		break;
	case pour_it:
		load_pour_it();
		// Reset initial values
		reset_values(false);
		initialized = false;

		stage_system->set_stage(StageSystem::Stage::turn_based);
		break;
	case milk_it:
		load_milk_it();
		// Reset initial values
		reset_values(false);
		initialized = false;

		stage_system->set_stage(StageSystem::Stage::turn_based);
		break;
	}

	// all minigame entities should now be shown
	for (Entity entity : registry.miniGame.entities) {
		if (registry.renderRequests.has(entity)) {
			RenderRequest& rr = registry.renderRequests.get(entity); 
			rr.shown = true;
		}
	}

	if (initialized)
		not_started = false;
}

void MinigameSystem::load_cool_it() {
	// Set description_text by reading in txt file
	std::string line;
	std::fstream myfile;
	myfile.open("..//..//..//src//stage_system//combat//cool_it_description.txt");
	float initial_y = window_height_px - 25;
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			// Create text instructions for game 
			Entity current_text = createText(line, { 10, initial_y },
				1.0f, not_selected_color, glm::mat4(1.0f), StageSystem::Stage::minigame, false);
			registry.textRenderRequests.get(current_text).shown = true;
			initial_y -= 25;

			// add the text to the map
			render_text_map[line] = current_text;
		}
	}
	myfile.close();

	render_text_map["practice"] = createText("Currently in PRACTICE MODE. Press 'S' to start!", {10, 10},
		1.0f, not_selected_color, glm::mat4(1.0f), StageSystem::Stage::minigame, false);
	registry.textRenderRequests.get(render_text_map["practice"]).shown = true;

	// create the normal state cup
	createCup(renderer, { x_center + 100, y_center }, curr_rhythm->rhythm_len, beat_error * 2);
	// creating perfect
	createMiniResult(renderer, { x_center / 2, y_center / 2 + 100}, beat_error * 2, minigame_state::perfect);
	// creating good
	createMiniResult(renderer, { x_center / 2, y_center / 2 + 200}, beat_error * 2, minigame_state::good);
	// creating fail
	createMiniResult(renderer, { x_center / 2, y_center / 2 + 300 }, beat_error * 2, minigame_state::fail);
	// creating cool cloud
	createMiniResult(renderer, { x_center / 2 + 700, y_center / 2 + 200 }, beat_error * 2, minigame_state::normal);
	// creating minigame good visualizer
	createMiniIndicator(renderer, { x_center / 2 + 700, y_center / 2 + 500 }, minigame_state::good);
	// creating minigame perfect visualizer
	createMiniIndicator(renderer, { x_center / 2 + 700, y_center / 2 + 500 }, minigame_state::perfect);
}

void MinigameSystem::load_pour_it() {
	printf("POUR IT NOT IMPLEMENTED YET SORRY!");
}

void MinigameSystem::load_milk_it() {
	printf("POUR IT NOT IMPLEMENTED YET SORRY!");
}

void MinigameSystem::set_current_game_color(vec3 color) {
	TextRenderRequest& trr = registry.textRenderRequests.get(minigame_text_map[selected_game]);
	trr.color = color;
}

void MinigameSystem::minigame_step(float elapsed_ms_since_last_update) {
	bool ended = false;

	// Handling for ending of minigame
	float minigame_timer_counter_ms = curr_rhythm->rhythm_len;
	for (Entity entity : registry.miniGameTimer.entities) {
		MiniGameTimer& timer = registry.miniGameTimer.get(entity);

		// handle interpolated image state
		if (!(timer.cup_state == minigame_state::normal)) {
			timer.inter_timer -= elapsed_ms_since_last_update;

			// reset cup image
			// cup interpolation should only last beat_error amount of length
			if (timer.inter_timer < 0) {
				registry.renderRequests.remove(entity); 
				RenderRequest& rr = registry.renderRequests.insert(
					entity,
					{ TEXTURE_ASSET_ID::MINIGAMECUP,
					EFFECT_ASSET_ID::TEXTURED,
					GEOMETRY_BUFFER_ID::SPRITE }
				); 
				rr.shown = true;
				timer.cup_state = minigame_state::normal;
				timer.inter_timer = beat_error;
			}
		}

		// set ended to true once timer reaches rhythm length
		if (minigame_overall_timer >= timer.counter_ms) {
			if (!practice) {
				// delete the cup
				registry.remove_all_components_of(entity);

				ended = true;

				// load an attack on enemies
				loaded = true;
			}
		}
	}

	// Handling for visual feedback of hits
	for (Entity entity : registry.miniGameResTimer.entities) {
		if (registry.renderRequests.has(entity)) {
			RenderRequest& rr = registry.renderRequests.get(entity);

			// if minigame is over, delete all the results
			if (ended && !practice) {
				registry.remove_all_components_of(entity);
			}
			else {
				// handling result timer
				if (rr.shown) {
					// results should only be shown every beat_duration at maximum; res timer max is only beat_error
					MiniGameResTimer& timer = registry.miniGameResTimer.get(entity);
					timer.counter_ms -= elapsed_ms_since_last_update;

					if (timer.counter_ms < 0) {
						rr.shown = false;
						timer.counter_ms = beat_error;
					}
				}
			}
		}
	}

	// Handling for visualizer of when to hit
	for (Entity entity : registry.miniGameVisual.entities) {
		MiniGameVisualizer& visual = registry.miniGameVisual.get(entity);

		if (registry.renderRequests.has(entity)) {
			// if minigame is over, delete all the results
			if (ended && !practice) {
				registry.remove_all_components_of(entity);
			}
			else {
				// check if we are on second or fourth beat right now
				minigame_state hit_state = calc_modded_beats();
				RenderRequest& rr = registry.renderRequests.get(entity);

				// if perfect 
				if (hit_state == minigame_state::perfect &&
					visual.res_state == minigame_state::perfect) {
					rr.shown = true;
					// if good
				}
				else if (hit_state == minigame_state::good &&
					visual.res_state == minigame_state::good) {
					rr.shown = true;
				}
				// not time to show
				else {
					rr.shown = false;
				}
			}
		}
	}

	// Handling for score 
	if (registry.textRenderRequests.has(render_text_map["score"])) {
		TextRenderRequest& trr = registry.textRenderRequests.get(render_text_map["score"]);
		trr.text = "Score: " + std::to_string(score);
	}

	// end minigame
	if (ended && !practice) {
		std::cout << "SCORE: " << score << '\n';

		// Reset initial values
		reset_values(false);
		initialized = false;

		stage_system->set_stage(StageSystem::Stage::turn_based);
	}
}

void MinigameSystem::reset_values(bool is_hard_reset) {
	not_started = true;
	practice = true;
	selected_game = cool_it;
	practice_music_start = false;
	minigame_music_start = false;
	time_since_last_press = 0.0f;
	minigame_overall_timer = 0.0f;

	if (is_hard_reset) {
		loaded = false;
		score = 0;
	}

	// default game selection colours map
	for (auto const& x : minigame_text_map)
	{
		if (registry.textRenderRequests.has(x.second)) {
			if (x.first == selected_game) {
				registry.textRenderRequests.get(x.second).color = selected_color;
			}
			else {
				registry.textRenderRequests.get(x.second).color = not_selected_color;
			}
		}
	}

	// clear text map
	for (auto const& x : render_text_map)
	{
		registry.remove_all_components_of(x.second);
	}
}

minigame_state MinigameSystem::calc_modded_beats() {
	float modded = (int)minigame_overall_timer % measure_duration;

	// For every second beat, the perfect value of modded is beat_duration
		// So then beat_duration - beat_error <= modded <= beat_duration + beat_error for a hit
	if (modded == beat_duration) {
		return minigame_state::perfect;
	}
	else if (modded > (beat_duration - beat_error) && modded < beat_duration) {
		return minigame_state::good;
	}
	else if (modded < (beat_duration + beat_error) && modded > beat_duration) {
		return minigame_state::good;
	}

	// For every fourth beat, the perfect value of modded is beat_duration * 3
		// So then (beat_duration * 3) - beat_error <= modded <= (beat_duration * 3) + beat_error for a hit
	if (modded == beat_duration * 3) {
		return minigame_state::perfect;
	}
	else if (modded > (beat_duration * 3 - beat_error) && modded < beat_duration * 3) {
		return minigame_state::good;
	}
	else if (modded < (beat_duration * 3 + beat_error) && modded > beat_duration * 3) {
		return minigame_state::good;
	}

	return minigame_state::fail;
}

void MinigameSystem::handle_mini() {
	bool hit = false;
	bool perfect_hit = false;

	// Handling for the tea cup and score settting
	for (Entity entity : registry.miniGameTimer.entities) {
		MiniGameTimer& timer = registry.miniGameTimer.get(entity);

		float modded = (int)minigame_overall_timer % measure_duration;

		std::cout << minigame_overall_timer << '\n';
		std::cout << modded << '\n';

		minigame_state hit_state = calc_modded_beats(); 

		if (hit_state == minigame_state::perfect) {
			perfect_hit = true;
		}
		else if (hit_state == minigame_state::good) {
			hit = true;
		}

		// Normal hits will increment score by 1; perfect hits will increment score by 2
		if (!practice) {
			// Score will only increment after first two measures 
			if (minigame_overall_timer > measure_duration * 2) {
				if (hit) {
					score++;
				}
				else if (perfect_hit) {
					score += 2;
				}
			}
		}

		// change cup's state on a hit
		if (hit || perfect_hit) {
			if (!(timer.cup_state == minigame_state::fail)) {
				registry.renderRequests.remove(entity);
				RenderRequest& rr = registry.renderRequests.insert(
					entity,
					{ TEXTURE_ASSET_ID::MINIGAMECUPGOOD,
					EFFECT_ASSET_ID::TEXTURED,
					GEOMETRY_BUFFER_ID::SPRITE }
				);
				rr.shown = true;
				timer.cup_state = minigame_state::good;
			}
		}
		else {
			registry.renderRequests.remove(entity);
			RenderRequest& rr = registry.renderRequests.insert(
				entity,
				{ TEXTURE_ASSET_ID::MINIGAMECUPBAD,
				EFFECT_ASSET_ID::TEXTURED,
				GEOMETRY_BUFFER_ID::SPRITE }
			);
			rr.shown = true;
			timer.cup_state = minigame_state::fail;
		}
	}

	// Handling for the visual feedback 
	for (Entity entity : registry.miniGameResTimer.entities) {
		MiniGameResTimer& res = registry.miniGameResTimer.get(entity);

		// Rendering cloud
		if (res.res_state == minigame_state::normal) {
			registry.renderRequests.get(entity).shown = true;
			continue;
		}

		// rendering results
		if (hit) {
			if (res.res_state == minigame_state::good) {
				registry.renderRequests.get(entity).shown = true;
			}
			else {
				registry.renderRequests.get(entity).shown = false;
			}
		} 
		else if (perfect_hit) {
			if (res.res_state == minigame_state::perfect) {
				registry.renderRequests.get(entity).shown = true;
			}
			else {
				registry.renderRequests.get(entity).shown = false;
			}
		}
		else {
			if (res.res_state == minigame_state::fail) {
				registry.renderRequests.get(entity).shown = true;
			}
			else {
				registry.renderRequests.get(entity).shown = false;
			}
		}
	}
}

void MinigameSystem::handle_minigame_key(int key, int action) {
	if (action == GLFW_RELEASE) {
		if (not_started) {
			switch (key) {
			case GLFW_KEY_LEFT: {
				set_current_game_color(not_selected_color);

				selected_game = static_cast<Minigame>(((selected_game - 1) % (milk_it + 1) + (milk_it + 1)) % (milk_it + 1));

				set_current_game_color(selected_color);
				break;
			} 
			case GLFW_KEY_RIGHT: {
				set_current_game_color(not_selected_color);

				selected_game = static_cast<Minigame>((selected_game + 1) % (milk_it + 1));

				set_current_game_color(selected_color);
				break;
			}
			case GLFW_KEY_ENTER:
				handle_game_selection();
				break;
			}
		}
		else {
			if (practice) {
				if (key == GLFW_KEY_S) {
					practice = false;
					// change the practice text
					registry.textRenderRequests.get(render_text_map["practice"]).text =
						"You get two measures to get the timing, good luck!";
					// add score text
					render_text_map["score"] = createText("Score: " + std::to_string(score), {window_width_px - 125, window_height_px - 25},
						1.0f, not_selected_color, glm::mat4(1.0f), StageSystem::Stage::minigame, false);
					registry.textRenderRequests.get(render_text_map["score"]).shown = true;

					// reset timer for key presses
					time_since_last_press = 0.0f;
					// reset overall timer 
					minigame_overall_timer = 0.0f;
				}
			}
			
			if (key == GLFW_KEY_D) {
				// key "D" will only be recognized for every beat
				if (time_since_last_press >= beat_duration) {
					// hard-coded bpm of metronome right now
					handle_mini();
					time_since_last_press = 0.0f;
				}
			}
		}
	}
}