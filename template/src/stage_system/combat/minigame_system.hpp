#pragma once 
#include "stage_system/stage_system.hpp"

class MinigameSystem 
{
public: 
	MinigameSystem(); 

	enum Rhythms {
		r1, r2
	};

	void init(StageSystem* stage_system_arg); 

	void handle_minigame_key(int key, int action);
private:
	bool started; 

	StageSystem* stage_system; 
};