#pragma once
#include <vector>

#include "tiny_ecs.hpp"
#include "components.hpp"

class ECSRegistry
{
	// Callbacks to remove a particular or all entities in the system
	std::vector<ContainerInterface*> registry_list;

public:
	// Manually created list of all components this game has
	ComponentContainer<DeathTimer> deathTimers;
	ComponentContainer<AttackTimer> attackTimers;
	ComponentContainer<Motion> motions;
	ComponentContainer<Collision> collisions;
	ComponentContainer<Player> players;
	ComponentContainer<Mesh*> meshPtrs;
	ComponentContainer<RenderRequest> renderRequests;
	ComponentContainer<TextRenderRequest> textRenderRequests;
	ComponentContainer<ScreenState> screenStates;
	ComponentContainer<Eatable> eatables;
	ComponentContainer<Deadly> deadlys;
	ComponentContainer<EnemyDrink> enemyDrinks;
	ComponentContainer<DebugComponent> debugComponents;
	ComponentContainer<vec3> colors;
	ComponentContainer<Menu> menu;
	ComponentContainer<MenuOption> menuOptions;
	ComponentContainer<Minigame> miniGame;
	ComponentContainer<MiniGameTimer> miniGameTimer;
	ComponentContainer<MiniGameResTimer> miniGameResTimer;
	ComponentContainer<PersistenceFeedbackTimer> persistanceFeedbackTimer;
	ComponentContainer<Background> backgrounds;
	ComponentContainer<Foreground> foregrounds;
	ComponentContainer<MainMenu> mainMenu;
	ComponentContainer<OverWorld> overWorld;
	ComponentContainer<TutorialBoard> tutorials;
	ComponentContainer<CutScene> cutscenes;
	ComponentContainer<TurnBased> turnBased;
	ComponentContainer<MiniStage> miniStage;
	ComponentContainer<MiniGameVisualizer> miniGameVisual;
	ComponentContainer<LevelNode> levelNode;

	// TURN BASED COMPONENTS
	ComponentContainer<CharacterData> characterDatas;
	ComponentContainer<TurnBasedEnemy> turnBasedEnemies;
	ComponentContainer<PartyMember> partyMembers;
	ComponentContainer<TurnCounter*> turnCounter;
	ComponentContainer<InjuredTimer> injuryTimers;
	ComponentContainer<HealthOutline> healthOutlines;
	ComponentContainer<HealthBarFill> healthBarFills;

	// constructor that adds all containers for looping over them
	// IMPORTANT: Don't forget to add any newly added containers!
	ECSRegistry()
	{
		// TODO: A1 add a LightUp component
		registry_list.push_back(&deathTimers);
		registry_list.push_back(&attackTimers);
		registry_list.push_back(&motions);
		registry_list.push_back(&collisions);
		registry_list.push_back(&players);
		registry_list.push_back(&meshPtrs);
		registry_list.push_back(&renderRequests);
		registry_list.push_back(&textRenderRequests);
		registry_list.push_back(&screenStates);
		registry_list.push_back(&eatables);
		registry_list.push_back(&deadlys);
		registry_list.push_back(&enemyDrinks);
		registry_list.push_back(&debugComponents);
		registry_list.push_back(&colors);
		registry_list.push_back(&backgrounds);
		registry_list.push_back(&foregrounds);
		registry_list.push_back(&mainMenu);
		registry_list.push_back(&overWorld);
		registry_list.push_back(&tutorials);
		registry_list.push_back(&cutscenes);
		registry_list.push_back(&turnBased);
		registry_list.push_back(&miniStage);
		registry_list.push_back(&miniGameVisual);
		registry_list.push_back(&levelNode);

		registry_list.push_back(&characterDatas);
		registry_list.push_back(&turnBasedEnemies);
		registry_list.push_back(&partyMembers);
		registry_list.push_back(&turnCounter);
		registry_list.push_back(&menu);
		registry_list.push_back(&menuOptions);
		registry_list.push_back(&miniGame);
		registry_list.push_back(&miniGameTimer);
		registry_list.push_back(&miniGameResTimer);
		registry_list.push_back(&persistanceFeedbackTimer);
		registry_list.push_back(&injuryTimers);
		registry_list.push_back(&healthOutlines);
		registry_list.push_back(&healthBarFills);
		
	}

	void clear_all_components() {
		for (ContainerInterface* reg : registry_list)
			reg->clear();
	}

	void list_all_components() {
		printf("Debug info on all registry entries:\n");
		for (ContainerInterface* reg : registry_list)
			if (reg->size() > 0)
				printf("%4d components of type %s\n", (int)reg->size(), typeid(*reg).name());
	}

	void list_all_components_of(Entity e) {
		printf("Debug info on components of entity %u:\n", (unsigned int)e);
		for (ContainerInterface* reg : registry_list)
			if (reg->has(e))
				printf("type %s\n", typeid(*reg).name());
	}

	void remove_all_components_of(Entity e) {
		for (ContainerInterface* reg : registry_list) {
			// preserve the character datas
			if (reg != &characterDatas || !partyMembers.has(e)) {
				reg->remove(e);
			}
		}
			
	}
};

extern ECSRegistry registry;