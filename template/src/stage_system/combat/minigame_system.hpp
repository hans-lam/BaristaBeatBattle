#pragma once 
#include "stage_system/stage_system.hpp"
#include "render_system.hpp"

class MinigameSystem 
{
public: 
	MinigameSystem(); 

	struct MinigameRhythm {
		int BPM; 
		// length is in ms
		float rhythm_len; 
		std::string name;
	};

	enum Minigame {
		cool_it, 
		pour_it, 
		milk_it
	};

	void init(StageSystem* stage_system_arg, RenderSystem* renderer_arg); 

	void handle_minigame_key(int key, int action);

	void handle_set_rhythm();

	bool get_not_started() const {
		return this->not_started;
	}

	bool get_practice() const {
		return this->practice;
	}

	int get_score() const {
		return this->score;
	}

	int get_selected_game() const {
		return this->selected_game;
	}

	void handle_game_selection();

	void load_cool_it();
	void load_pour_it(); 
	void load_milk_it();

	bool initialized;
	bool loaded;
	bool practice_music_start;
	bool minigame_music_start;
	float time_since_last_press;
	float minigame_overall_timer;

	void minigame_step(float elapsed_ms_since_last_update);

	void reset_values(bool is_hard_reset);

	minigame_state calc_modded_beats() const;
private:
	void set_current_text_and_desc(vec3 color, bool shown);

	void read_game_desc(Minigame chosen_game);

	void start_minigame();

	void handle_minigame(int key);

	void handle_cool();
	bool step_cool(float elapsed_ms_since_last_update);

	void handle_pour();
	bool step_pour(float elapsed_ms_since_last_update);

	void set_milk_color(vec3 color);
	void handle_milk();
	bool step_milk(float elapsed_ms_since_last_update);

	bool not_started; 
	StageSystem* stage_system; 
	RenderSystem* renderer;

	// selection of minigame for user
	Minigame selected_game;
	std::map<Minigame, Entity> minigame_text_map;
	std::map<Minigame, Entity> minigame_desc_map;
	Entity minigame_select_title;
	// green
	glm::vec3 selected_color = glm::vec3(0.0, 1.0, 0.0);
	// black
	glm::vec3 not_selected_color = glm::vec3(0, 0, 0);
	// white
	glm::vec3 milk_selected_color = glm::vec3(1.0, 1.0, 1.0);
	// red
	glm::vec3 milk_result_color = glm::vec3(1.0, 0.0, 0.0);
	const int x_center = window_width_px / 2 - 100;
	const int y_center = window_height_px / 2;

	// Rhythm selection for minigame
	// All of this is hard-coded right now; not sure how we might dynamically set these values
	MinigameRhythm r0 = { 120, 12000.f, "120_bpm_with_metronome" };
	MinigameRhythm r1 = { 100, 15000.f, "placeholder_1" };
	MinigameRhythm r2 = { 100, 15000.f, "placeholder_2" };
	MinigameRhythm r3 = { 100, 15000.f, "placeholder_3" };
	MinigameRhythm rhythms_arr[4] = { r0, r1, r2, r3 };
	MinigameRhythm* curr_rhythm;

	bool practice;
	std::map<std::string, Entity> render_text_map;
	// score = curr_score / max_score for fractional damage calc
	// can be implemented at a later date
	int score;
	// for cool
	int beat_duration;
	int beat_error;
	int measure_duration;
	// for pour
	int target_pour = 0;
	int prev_pour = 0;
	// for milk
	enum RiddleOption {A, B, C, D};
	std::string options_arr[4] = { "A", "B", "C", "D" };
	struct Riddle {
		std::string answer;
		std::string question;
		std::string options[4];
	};
	Riddle rid1 = {
		"B", "What percentage of the world is lactose intolerant?",
		{"57%", "68%", "76%", "85%"}
	};
	Riddle rid2 = {
		"D", "Which country is the world's largest producer of milk?",
		{"USA", "France", "Italy", "India"}
	};
	Riddle rid3 = {
		"C", "How much milk do is required to make a pound of cheese?",
		{"20 lbs", "15 lbs", "10 lbs", "5 lbs"}
	};
	Riddle rid4 = {
		"A", "How big is the milky way's diamater (in light years)?",
		{"1e5", "1e7", "1e10", "1e12"}
	};
	Riddle riddles_arr[4] = { rid1, rid2, rid3, rid4 };
	Riddle current_riddle;
	RiddleOption chosen_answer;
};