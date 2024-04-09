#pragma once

#include <array>
#include <utility>

#include "common.hpp";
#include "components.hpp";
#include "tiny_ecs.hpp";
#include "stage_system/stage_system.hpp";

struct TextChar {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
	char character;
};

// System responsible for setting up OpenGL and for rendering all the
// visual entities in the game
class RenderSystem {
	/**
	 * The following arrays store the assets the game will use. They are loaded
	 * at initialization and are assumed to not be modified by the render loop.
	 *
	 * Whenever possible, add to these lists instead of creating dynamic state
	 * it is easier to debug and faster to execute for the computer.
	 */
	std::array<GLuint, texture_count> texture_gl_handles;
	std::array<ivec2, texture_count> texture_dimensions;

	//dummy vao
	GLuint dummy_VAO;

	// fonts
	std::map<char, TextChar> m_ftCharacters;
	GLuint m_font_shaderProgram;
	GLuint m_font_VAO;
	GLuint m_font_VBO;

	std::string font_filename = "..//..//..//data//fonts//Kenney_Pixel_Square.ttf";
	unsigned int font_default_size = 24;

	// Make sure these paths remain in sync with the associated enumerators.
	// Associated id with .obj path
	const std::vector < std::pair<GEOMETRY_BUFFER_ID, std::string>> mesh_paths =
	{
		  std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::CHICKEN, mesh_path("chicken.obj")),
		  std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::PLAYER, mesh_path("player.obj"))
		  // specify meshes of other assets here
	};

	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, texture_count> texture_paths = {
			textures_path("bug.png"),
			textures_path("eagle.png"),
			textures_path("enemydrink.png"),
			textures_path("battle_tutorial.png"),
			textures_path("overworld_tutorial.png"),
			textures_path("gameover_screen.png"),
			textures_path("attackbutton.png"),
			textures_path("itembutton.png"),
			textures_path("restbutton.png"),
			textures_path("cool_box_perfect.png"),
			textures_path("cool_box_good.png"), 
			textures_path("cool_cloud.png"),
			textures_path("minigame_cup.png"),
			textures_path("minigame_cup_good.png"),
			textures_path("minigame_cup_bad.png"),
			textures_path("hit_box_perfect.png"),
			textures_path("hit_box_good.png"),
			textures_path("hit_box_fail.png"),
			textures_path("kettle_normal.png"),
			textures_path("kettle_little_more.png"),
			textures_path("kettle_lot_more.png"),
			textures_path("kettle_just_right.png"),
			textures_path("kettle_pour.png"),
			textures_path("kettle_dead.png"),
			textures_path("milk_almond.png"),
			textures_path("milk_2.png"), 
			textures_path("milk_coconut.png"), 
			textures_path("milk_soy.png"),
			textures_path("mini_speech.png"),
			textures_path("bg.png"),
			textures_path("fg.png"),
			textures_path("fglight.png"),
			textures_path("bg_battle.png"),
			textures_path("spritesheet.png"),
			textures_path("health_bar_empty.png"),
			textures_path("fill_bar.png"),
			textures_path("cutscenebg.png"),
			textures_path("cutsceneBox1.png"),
			textures_path("cutsceneBox2.png"),
			textures_path("cutsceneBox3.png"),
			textures_path("earlgrey.png"),
			textures_path("americano.png"),

	};

	std::array<GLuint, effect_count> effects;
	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, effect_count> effect_paths = {
		shader_path("coloured"),
		shader_path("egg"),
		shader_path("chicken"),
		shader_path("textured"),
		shader_path("wind"),
		shader_path("background"),
		shader_path("foreground"),
		shader_path("lights"),
		shader_path("battle"),
		shader_path("battlebar"),
	};

	std::array<GLuint, geometry_count> vertex_buffers;
	std::array<GLuint, geometry_count> index_buffers;
	std::array<Mesh, geometry_count> meshes;

public:
	// Initialize the window
	bool init(GLFWwindow* window);

	template <class T>
	void bindVBOandIBO(GEOMETRY_BUFFER_ID gid, std::vector<T> vertices, std::vector<uint16_t> indices);

	void initializeGlTextures();

	void initializeGlEffects();

	void initializeGlMeshes();

	bool fontInit();
	Mesh& getMesh(GEOMETRY_BUFFER_ID id) { return meshes[(int)id]; };

	void initializeGlGeometryBuffers();
	// Initialize the screen texture used as intermediate render target
	// The draw loop first renders to this texture, then it is used for the wind
	// shader
	bool initScreenTexture();

	// Destroy resources associated to one or all entities created by the system
	~RenderSystem();

	// Draw all entities
	void draw(StageSystem::Stage current_stage);

	// Render text
	void renderText(const std::string& text, float x, float y,
		float scale, const glm::vec3& color,
		const glm::mat4& trans);

	mat3 createProjectionMatrix();

private:
	// Internal drawing functions for each entity type
	void drawTexturedMesh(Entity entity, const mat3& projection);
	void drawToScreen();

	// Window handle
	GLFWwindow* window;

	// Screen texture handles
	GLuint frame_buffer;
	GLuint off_screen_render_buffer_color;
	GLuint off_screen_render_buffer_depth;

	Entity screen_state_entity;
};

bool loadEffectFromFile(
	const std::string& vs_path, const std::string& fs_path, GLuint& out_program);
