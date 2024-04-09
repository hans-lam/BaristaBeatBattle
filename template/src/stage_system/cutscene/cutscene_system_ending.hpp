#pragma once
#include "stage_system/stage_system.hpp"
#include "common.hpp"

#include "render_system.hpp"
#include "world_init.hpp"

class CutSceneSystemEnding
{
public: 
	CutSceneSystemEnding();


	void init(StageSystem* stage_system_arg, RenderSystem* renderer);

	void handle_cutscene_keys(int key, int action); 

	void handle_cutscene();

	void handle_cutscene_render(RenderSystem* renderer);

	Entity  createCharPic(RenderSystem* renderer, vec2 pos, int character);

	

private:
	const float min_slide_counter_ms = 10000.f;


	Entity textBox;
	Entity char1;
	Entity char2;


	StageSystem* stage_system;
	int cutscene_slide = 1; 
	int cutscene_currently_rendered = 0;
	bool cutscene_done;
	bool skip_cutscene;
};