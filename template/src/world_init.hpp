#pragma once

#include "common.hpp"
#include "tiny_ecs.hpp"
#include "render_system.hpp"
#include "turn_based_system/character_factory/character_factory.hpp"

// These are ahrd coded to the dimensions of the entity texture
const float BUG_BB_WIDTH = 0.6f * 165.f;
const float BUG_BB_HEIGHT = 0.6f * 165.f;
const float EAGLE_BB_WIDTH = 0.6f * 300.f;
const float EAGLE_BB_HEIGHT = 0.6f * 300.f;
const float MENU_WIDTH = 0.6f * 700.f; 
const float MENU_HEIGHT = 0.6f * 280.f;
const float CUP_WIDTH = 0.8f * 700.f;
const float CUP_HEIGHT = 0.8f * 550.f;
const float BG_WIDTH = 2000.f;
const float BG_HEIGHT = 350.f;
const float FG_WIDTH = 2000.f;
const float FG_HEIGHT = 550.f;
const float HEALTH_BAR_WIDTH = 250.f;
const float HEALTH_BAR_HEIGHT = 62.5f;
const float FILL_WIDTH = 135.f;
const float FILL_HEIGHT = 18.f;
const float TEXTBOX_WIDTH = 900.f;
const float TEXTBOX_HEIGHT = 300.f;

// the player
Entity createChicken(RenderSystem* renderer, vec2 pos);
// Menu
Entity createMenu(RenderSystem* renderer, vec2 pos, Entity associated_character);
// the prey
Entity createBug(RenderSystem* renderer, vec2 position);
// minigame
Entity createCup(RenderSystem* renderer, vec2 pos, float length, float inter_timer);
// minigame result
Entity createMiniResult(RenderSystem* renderer, vec2 pos, float inter_timer, minigame_state mini_res);
// minigame hit indicator 
Entity createMiniIndicator(RenderSystem* renderer, vec2 pos, minigame_state mini_res);
// the enemy
Entity createEagle(RenderSystem* renderer, vec2 position);
// the real enemy
Entity createEnemyDrink(RenderSystem* renderer, vec2 velocity, vec2 position);
// the background scroller
Entity createBackgroundScroller(RenderSystem* renderer, vec2 position);
// the foreground scoller(s)
Entity createForegroundScroller(RenderSystem* renderer, vec2 position, bool isLight);
// minigame background
Entity createBackgroundBattle(RenderSystem* renderer, vec2 position);
// cutscene background 
Entity createBackgroundCutscene(RenderSystem* renderer, vec2 position);
// Main Menu
Entity createMainMenu(RenderSystem* renderer, vec2 position);
// Tutorial
Entity createTutorialWindow(RenderSystem* renderer, vec2 position, int window);
// LevelNode
Entity createLevelNode(RenderSystem* renderer, int level_num, vec2 position);
// a red line for debugging purposes
Entity createLine(vec2 position, vec2 scale, float angle);
// a egg
Entity createEgg(vec2 pos, vec2 size);

Entity create_chai(RenderSystem* renderer, vec2 pos);
Entity create_americano(RenderSystem* renderer, vec2 pos);
Entity create_earl(RenderSystem* renderer, vec2 pos);
Entity create_london(RenderSystem* renderer, vec2 pos);
Entity create_turn_based_enemy(RenderSystem* renderer, vec2 pos, int level);
Entity create_health_bar_outline(RenderSystem* renderer, vec2 pos);
Entity create_cutscene_text_box(RenderSystem* renderer, int selection,  vec2 pos, vec2 textPos, std::string text, std::string text2, std::string text3,  float scale, vec3 color, glm::mat4 trans, StageSystem::Stage current_stage);
Entity create_health_bar_fill(RenderSystem* renderer, vec2 pos, Entity associated_character);

Entity createText(std::string text, vec2 position, float scale, vec3 color, glm::mat4 trans, StageSystem::Stage current_stage);
