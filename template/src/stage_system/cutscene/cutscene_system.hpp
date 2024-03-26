#pragma once
#include "stage_system/stage_system.hpp"

class CutSceneSystem
{
public: 
	CutSceneSystem(); 

	void init(StageSystem* stage_system_arg);

	void handle_cutscene_keys(int key, int action); 

	void handle_cutscene();

	void handle_cutscene_timer(float elapsed_ms_since_last_update);
private:
	StageSystem* stage_system;
	bool cutscene_done;
	bool skip_cutscene;
};