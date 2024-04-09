#pragma once
#include "stage_system/stage_system.hpp"
#include "common.hpp"

#include "render_system.hpp"
#include "world_init.hpp"

class CutSceneSystem
{
public: 
	CutSceneSystem(); 


	void init(StageSystem* stage_system_arg, RenderSystem* renderer);

	void handle_cutscene_keys(int key, int action); 

	void handle_cutscene();

	void handle_cutscene_render(RenderSystem* renderer, int level, bool recruited);

	Entity createCharPic(RenderSystem* renderer, vec2 pos, int character);

	

private:
	const float min_slide_counter_ms = 10000.f;

	Entity textBox;
	Entity textBox1;
	Entity textBox2;
	Entity char1;
	Entity char2;
	Entity char3;

	StageSystem* stage_system;
	int cutscene_slide = 1; 
	int slides = 5; 
	int cutscene_currently_rendered = 0;
	bool cutscene_done;
	bool skip_cutscene;
};