#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"
#include <iostream>


Entity createChicken(RenderSystem* renderer, vec2 pos)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::CHICKEN);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = mesh.original_size * 300.f;
	motion.scale.y *= -1; // point front to the right

	registry.overWorld.emplace(entity);
	registry.players.emplace(entity);
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::PLAYER, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::CHICKEN, // shuold prob fix this later
			GEOMETRY_BUFFER_ID::PLAYER });

	return entity;
}

Entity createBug(RenderSystem* renderer, vec2 position)
{
	// Reserve en entity
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the position, scale, and physics components
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 50 };
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -BUG_BB_WIDTH, BUG_BB_HEIGHT });

	// Create an (empty) Bug component to be able to refer to all bug
	registry.eatables.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BUG,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createCup(RenderSystem* renderer, vec2 pos) {
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = pos;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -CUP_WIDTH, CUP_HEIGHT });

	// Place in minigame
	registry.miniGame.emplace(entity);
	// Place in minigame timer for ryhthym calcs
	registry.miniGameTimer.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MINIGAMECUP,
		EFFECT_ASSET_ID::TEXTURED,
		GEOMETRY_BUFFER_ID::SPRITE }
	);

	return entity;
}

Entity createMiniResult(RenderSystem* renderer, vec2 pos) {
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = pos;

	motion.scale = vec2({ MENU_WIDTH, MENU_HEIGHT });
	// place in minigame result
	registry.miniGameResTimer.emplace(entity);
	RenderRequest& render = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::MINIGAMEFAIL,
		EFFECT_ASSET_ID::TEXTURED,
		GEOMETRY_BUFFER_ID::SPRITE }
	);
	return entity;
}

Entity createEagle(RenderSystem* renderer, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 100.f };
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -EAGLE_BB_WIDTH, EAGLE_BB_HEIGHT });

	// Create and (empty) Eagle component to be able to refer to all eagles
	registry.deadlys.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::EAGLE,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}


Entity createEnemyDrink(RenderSystem* renderer, vec2 velocity, vec2 position)
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = velocity;
	motion.position = position;

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -EAGLE_BB_WIDTH, EAGLE_BB_HEIGHT });

	registry.overWorld.emplace(entity);
	registry.enemyDrinks.emplace(entity);
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ENEMYDRINK,
		 EFFECT_ASSET_ID::FOREGROUND,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.shown = true;

	return entity;
}

Entity createMenu(RenderSystem* renderer, vec2 pos, Entity associated_character)
{
	auto menuEnt = Entity();
	auto attack = Entity();
	auto rest = Entity();
	auto pourIt = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& atkMesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(attack, &atkMesh);
	Mesh& restMesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(rest, &restMesh);
	Mesh& pourItMesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(pourIt, &pourItMesh);

	MenuOption& pourItOp = registry.menuOptions.emplace(pourIt);
	registry.colors.insert(pourIt, { 1, 0.8f, 0.8f });
	pourItOp.option = "pour it";
	MenuOption& attackOp = registry.menuOptions.emplace(attack);
	registry.colors.insert(attack, { 1, 0.8f, 0.8f });
	attackOp.option = "attack";
	MenuOption& restOp = registry.menuOptions.emplace(rest);
	registry.colors.insert(rest, { 1, 0.8f, 0.8f });
	restOp.option = "rest";

	Menu& menu = registry.menu.emplace(menuEnt);
	menu.options[0] = attack;
	menu.options[1] = rest;
	menu.options[2] = pourIt;
	menu.activeOption = attack;

	menu.associated_character = associated_character;

	vec2 menuPos = pos;
	// Initialize the position, scale, and physics components
	auto& pourItMotion = registry.motions.emplace(pourIt);
	pourItMotion.angle = 0.f;
	pourItMotion.velocity = { 0, 0 };
	pourItMotion.position = { menuPos.x, menuPos.y - 108.f };
	pourItMotion.scale = vec2({ MENU_WIDTH, MENU_HEIGHT });

	auto& atkMotion = registry.motions.emplace(attack);
	atkMotion.angle = 0.f;
	atkMotion.velocity = { 0, 0 };
	// I am aware the 54 is a magic floating number, not sure how to calculate it as of rn
	atkMotion.position = { menuPos.x, menuPos.y - 54.f };
	atkMotion.scale = vec2({ MENU_WIDTH, MENU_HEIGHT });

	auto& restMotion = registry.motions.emplace(rest);
	restMotion.angle = 0.f;
	restMotion.velocity = { 0, 0 };
	restMotion.position = menuPos;
	restMotion.scale = vec2({ MENU_WIDTH, MENU_HEIGHT });

	registry.turnBased.emplace(menuEnt);
	registry.turnBased.emplace(attack);
	registry.turnBased.emplace(rest);
	registry.turnBased.emplace(pourIt);

	return menuEnt;
}

Entity createBackgroundScroller(RenderSystem* renderer, vec2 position) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// scale the background
	motion.scale = vec2({ BG_WIDTH, BG_HEIGHT });

	registry.overWorld.emplace(entity);
	registry.backgrounds.emplace(entity);
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BGSCROLL,
		 EFFECT_ASSET_ID::BACKGROUND,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createForegroundScroller(RenderSystem* renderer, vec2 position, bool isLight) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	registry.overWorld.emplace(entity);
	registry.foregrounds.emplace(entity);

	if (isLight) {
		// scale the background
		motion.scale = vec2({ BG_WIDTH, BG_HEIGHT });
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::FGLIGHT,
			 EFFECT_ASSET_ID::LIGHTS,
			 GEOMETRY_BUFFER_ID::SPRITE });
	}
	else {
		// scale the background
		motion.scale = vec2({ FG_WIDTH, FG_HEIGHT });
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::FGSCROLL,
			 EFFECT_ASSET_ID::FOREGROUND,
			 GEOMETRY_BUFFER_ID::SPRITE });
	}

	return entity;
}

Entity createBackgroundBattle(RenderSystem* renderer, vec2 position) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// scale the background
	motion.scale = vec2({ 1700, window_height_px });

	registry.turnBased.emplace(entity);
	registry.backgrounds.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BGBATTLE,
		 EFFECT_ASSET_ID::BATTLE,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}


Entity createBackgroundCutscene(RenderSystem* renderer, vec2 position) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// scale the background
	motion.scale = vec2({ 1700, window_height_px });

	registry.cutscenes.emplace(entity);
	registry.backgrounds.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BGCUTSECNE ,
		 EFFECT_ASSET_ID::BATTLE,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity createMainMenu(RenderSystem* renderer, vec2 position) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = position;

	// TODO: CHANGE MAIN MENU TEXTURE RENDERING HERE
	motion.scale = vec2({ -EAGLE_BB_WIDTH, EAGLE_BB_HEIGHT });
	registry.mainMenu.emplace(entity);
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ENEMYDRINK,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.shown = true;
	
	return entity;
}

Entity createTutorialWindow(RenderSystem* renderer, vec2 position, int window) {
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	Motion& motion = registry.motions.emplace(entity);
	motion.position = position;
	motion.scale = vec2({ MENU_WIDTH * 2, MENU_HEIGHT * 2 + 100 }); // ??? i don't think this is right
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	if (window == 1) {
		registry.overWorld.emplace(entity);
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::TUTORIALBOARD,
			  EFFECT_ASSET_ID::TEXTURED,
			  GEOMETRY_BUFFER_ID::SPRITE });

	}
	else if (window == 2) {
		registry.turnBased.emplace(entity);
		registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::BATTLEBOARD,
			  EFFECT_ASSET_ID::TEXTURED,
			  GEOMETRY_BUFFER_ID::SPRITE });

	}

	registry.tutorials.emplace(entity);

	return entity;
}

Entity createLine(vec2 position, vec2 scale, float angle)
{
	Entity entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT,
		 EFFECT_ASSET_ID::EGG,
		 GEOMETRY_BUFFER_ID::DEBUG_LINE });

	// Create motion
	Motion& motion = registry.motions.emplace(entity);
	motion.angle = angle;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;

	registry.debugComponents.emplace(entity);
	return entity;
}

Entity createEgg(vec2 pos, vec2 size)
{
	auto entity = Entity();

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = size;

	// Create and (empty) Chicken component to be able to refer to all eagles
	registry.deadlys.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::TEXTURE_COUNT, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::EGG,
			GEOMETRY_BUFFER_ID::EGG });

	return entity;
}



Entity create_chai(RenderSystem* renderer, vec2 pos) {

	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::CHICKEN);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = mesh.original_size * 300.f;
	motion.scale.y *= -1; // point front to the right

	// Create and (empty) Chicken component to be able to refer to all eagles
	registry.players.emplace(entity);
	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::PLAYER, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::CHICKEN, // shuold prob fix this later
			GEOMETRY_BUFFER_ID::PLAYER });
	rr.shown = true;


	// give entity turn based components
	character_factory.construct_chai(entity);

	registry.colors.insert(entity, { 1, 0.8f, 0.8f });

	registry.turnBased.emplace(entity);

	return entity;
}
Entity create_americano(RenderSystem* renderer, vec2 pos) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::CHICKEN);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = mesh.original_size * 300.f;
	motion.scale.y *= -1; // point front to the right


	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::PLAYER, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::CHICKEN, // shuold prob fix this later
			GEOMETRY_BUFFER_ID::PLAYER });
	rr.shown = true;

	// give entity turn based components
	character_factory.construct_americano(entity);

	registry.colors.insert(entity, { 1, 0.8f, 0.8f });

	registry.turnBased.emplace(entity);

	return entity;
}
Entity create_earl(RenderSystem* renderer, vec2 pos) {

	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::CHICKEN);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = mesh.original_size * 300.f;
	motion.scale.y *= -1; // point front to the right


	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::PLAYER, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::CHICKEN, // shuold prob fix this later
			GEOMETRY_BUFFER_ID::PLAYER });
	rr.shown = true;

	// give entity turn based components
	character_factory.construct_earl(entity);

	registry.colors.insert(entity, { 1, 0.8f, 0.8f });

	registry.turnBased.emplace(entity);

	return entity;
}

Entity create_london(RenderSystem* renderer, vec2 pos) {

	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::CHICKEN);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.scale = mesh.original_size * 300.f;
	motion.scale.y *= -1; // point front to the right


	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::PLAYER, // TEXTURE_COUNT indicates that no txture is needed
			EFFECT_ASSET_ID::CHICKEN, // shuold prob fix this later
			GEOMETRY_BUFFER_ID::PLAYER });

	// give entity turn based components
	character_factory.construct_london(entity);

	registry.colors.insert(entity, { 1, 0.8f, 0.8f });

	return entity;
}

Entity create_turn_based_enemy(RenderSystem* renderer, vec2 pos, int level) {

	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::CHICKEN);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.position = pos;
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };

	// Setting initial values, scale is negative to make it face the opposite way
	motion.scale = vec2({ -EAGLE_BB_WIDTH, EAGLE_BB_HEIGHT });

	RenderRequest& rr = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::ENEMYDRINK,
		 EFFECT_ASSET_ID::BATTLE,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.shown = true;

	// give entity turn based components
	character_factory.construct_enemy(entity, level);

	registry.colors.insert(entity, { 1, 0.8f, 0.8f });

	registry.turnBased.emplace(entity);

	return entity;
}

Entity createText(std::string text, vec2 position, float scale, vec3 color, glm::mat4 trans, StageSystem::Stage current_stage) {
	auto entity = Entity();
	auto& textRenderRequest = registry.textRenderRequests.emplace(entity);
	textRenderRequest.text = text;
	textRenderRequest.position = position;
	textRenderRequest.scale = scale;
	textRenderRequest.color = color;
	textRenderRequest.trans = trans;

	// Adding text to specified stage
	switch (current_stage) {
	case StageSystem::Stage::main_menu: 
		registry.mainMenu.emplace(entity); 
		break;
	case StageSystem::Stage::overworld:
		registry.overWorld.emplace(entity); 
		break;
	case StageSystem::Stage::cutscene:
		registry.cutscenes.emplace(entity);
		break;
	case StageSystem::Stage::turn_based:
		registry.turnBased.emplace(entity); 
		break;
	case StageSystem::Stage::minigame:
		registry.miniGame.emplace(entity);
		break;
	}

	return entity;
}

Entity create_health_bar_outline(RenderSystem* renderer, vec2 pos) {
	
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = pos;

	// scale the background
	motion.scale = vec2({ HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT });

	// Create and (empty) Eagle component to be able to refer to all eagles
	registry.healthOutlines.emplace(entity);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::HEALTHOUTLINE,
		 EFFECT_ASSET_ID::BATTLE,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}

Entity create_health_bar_fill(RenderSystem* renderer, vec2 pos, Entity associated_character) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = pos;

	// scale the background
	motion.scale = vec2({ FILL_WIDTH, FILL_HEIGHT });

	// Create and (empty) Eagle component to be able to refer to all eagles
	HealthBarFill& fill = registry.healthBarFills.emplace(entity);
	fill.associated_char = associated_character;
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::HEALTHFILL,
		 EFFECT_ASSET_ID::BATTLEBAR,
		 GEOMETRY_BUFFER_ID::SPRITE });

	return entity;
}


Entity create_cutscene_text_box(RenderSystem* renderer, int selection, vec2 pos, vec2 textPos, std::string text, std::string text2, std::string text3 , float scale, vec3 color, glm::mat4 trans, StageSystem::Stage current_stage) {
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Initialize the motion
	auto& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0.f, 0.f };
	motion.position = pos;

	// scale the background
	motion.scale = vec2({ TEXTBOX_WIDTH, TEXTBOX_HEIGHT });

	//add the text
	/*Entity text1 = createText( text, textPos,  scale,  color,  trans, current_stage);
	Entity text2 = createText(text2, { textPos.x, textPos.y-40}, scale, color, trans, current_stage);
	Entity text3 = createText(text3, { textPos.x, textPos.y -80 }, scale, color, trans, current_stage);
*/

	// Create and (empty) Eagle component to be able to refer to all eagles
	registry.cutscenes.emplace(entity);
	
	if (selection == 1) {
		RenderRequest&		rq = registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::CUTSCENETEXTBOX1,
			 EFFECT_ASSET_ID::BATTLE,
			 GEOMETRY_BUFFER_ID::SPRITE });
		rq.shown = true;
	}
	else if (selection == 2) {
		RenderRequest& 		rq = registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::CUTSCENETEXTBOX2,
			 EFFECT_ASSET_ID::BATTLE,
			 GEOMETRY_BUFFER_ID::SPRITE });
		rq.shown = true;
	}
	else if (selection == 3) {
		RenderRequest&		rq = registry.renderRequests.insert(
			entity,
			{ TEXTURE_ASSET_ID::CUTSCENETEXTBOX3,
			 EFFECT_ASSET_ID::BATTLE,
			 GEOMETRY_BUFFER_ID::SPRITE });
		rq.shown = true;
	}
	else {
	RenderRequest& rq = registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::CUTSCENETEXTBOX3,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rq.shown = true;
	}
	
	return entity;
}