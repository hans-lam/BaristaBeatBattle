// internal
#include "render_system.hpp"
#include <SDL.h>
#include "tiny_ecs_registry.hpp"

// matrices
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// stlib
#include <iostream>
#include <sstream>


void RenderSystem::drawTexturedMesh(Entity entity,
									const mat3 &projection)
{
	Motion &motion = registry.motions.get(entity);
	// Transformation code, see Rendering and Transformation in the template
	// specification for more info Incrementally updates transformation matrix,
	// thus ORDER IS IMPORTANT
	Transform transform;
	transform.translate(motion.position);

	transform.rotate(motion.angle);

	transform.scale(motion.scale);

	assert(registry.renderRequests.has(entity));
	const RenderRequest &render_request = registry.renderRequests.get(entity);

	if (!(render_request.shown)) {
		return;
	}

	const GLuint used_effect_enum = (GLuint)render_request.used_effect;
	assert(used_effect_enum != (GLuint)EFFECT_ASSET_ID::EFFECT_COUNT);
	const GLuint program = (GLuint)effects[used_effect_enum];

	// Setting shaders
	glUseProgram(program);
	gl_has_errors();

	assert(render_request.used_geometry != GEOMETRY_BUFFER_ID::GEOMETRY_COUNT);
	const GLuint vbo = vertex_buffers[(GLuint)render_request.used_geometry];
	const GLuint ibo = index_buffers[(GLuint)render_request.used_geometry];

	// Setting vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	gl_has_errors();

	// Input data location as in the vertex buffer
	//if (render_request.used_effect == EFFECT_ASSET_ID::TEXTURED)
	//{
	//	GLint in_position_loc = glGetAttribLocation(program, "in_position");
	//	GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
	//	gl_has_errors();
	//	assert(in_texcoord_loc >= 0);

	//	//glGenVertexArrays(1, &dummy_VAO);
	//	glBindVertexArray(dummy_VAO);

	//	glEnableVertexAttribArray(in_position_loc);
	//	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
	//						  sizeof(TexturedVertex), (void *)0);
	//	gl_has_errors();

	//	glEnableVertexAttribArray(in_texcoord_loc);
	//	glVertexAttribPointer(
	//		in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
	//		(void *)sizeof(
	//			vec3)); // note the stride to skip the preceeding vertex position

	//	// Enabling and binding texture to slot 0
	//	glActiveTexture(GL_TEXTURE0);
	//	gl_has_errors();

	//	assert(registry.renderRequests.has(entity));
	//	GLuint texture_id =
	//		texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];

	//	glBindTexture(GL_TEXTURE_2D, texture_id);
	//	gl_has_errors();
	//}
	//else 
		
	if (render_request.used_effect == EFFECT_ASSET_ID::CHICKEN)
	{
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_color_loc = glGetAttribLocation(program, "in_color");
		gl_has_errors();

		GLuint time_uloc = glGetUniformLocation(program, "time");
		glUniform1f(time_uloc, (float)(glfwGetTime() * 10.0f));
		gl_has_errors();

		GLint curr_frame_uloc = glGetUniformLocation(program, "curr_frame");

		// determine whether idle, walking +x, -x, attacking
		// fix this later
		if (registry.attackTimers.has(entity)) {
			glUniform1i(curr_frame_uloc, 1);	// attacking
		}
		else {
			vec2 player_velocity = registry.motions.get(entity).velocity;
			if (player_velocity == vec2(0.0, 0.0)) {
				glUniform1i(curr_frame_uloc, 0);	// idle
			}
			else if (player_velocity.x > 0.f) {
				glUniform1i(curr_frame_uloc, 4);	// moving right
			}
			else if (player_velocity.x < 0.f) {
				glUniform1i(curr_frame_uloc, 6);	// moving left
			}
			else if (player_velocity.y > 0.f) {
				glUniform1i(curr_frame_uloc, 2);	// moving up
			}
			else if (player_velocity.y < 0.f) {
				glUniform1i(curr_frame_uloc, 8);	// moving down
			}
			else {
				glUniform1i(curr_frame_uloc, 0);	// idle
			}
		}

		gl_has_errors();

		// Enabling and binding texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();

		assert(registry.renderRequests.has(entity));
		GLuint texture_id =
			texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glUniform1i(glGetUniformLocation(program, "sprite_sheet"), 0);

		gl_has_errors();

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
			sizeof(ColoredVertex), (void*)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_color_loc);
		glVertexAttribPointer(in_color_loc, 3, GL_FLOAT, GL_FALSE,
			sizeof(ColoredVertex), (void*)sizeof(vec3));
		assert(in_color_loc > -1);
		gl_has_errors();

		// is the injury timer on
		GLint injured_uloc = glGetUniformLocation(program, "is_injured");
		GLfloat redness_uloc = glGetUniformLocation(program, "redness");
		GLuint counter_uloc = glGetUniformLocation(program, "counter");
		assert(counter_uloc >= 0);
		assert(injured_uloc >= 0);
		assert(redness_uloc >= 0);

		gl_has_errors();

		if (registry.injuryTimers.has(entity)) {
			InjuredTimer& injury = registry.injuryTimers.get(entity);
			glUniform1i(injured_uloc, 1);
			glUniform1f(redness_uloc, injury.redness_factor);
			glUniform1f(counter_uloc, (3000.f - injury.counter_ms) / 3000.f);
		}
		else {
			glUniform1i(injured_uloc, 0);
			glUniform1f(redness_uloc, 0.0);
		}

		gl_has_errors();

	}
	else if (render_request.used_effect == EFFECT_ASSET_ID::SPARKLE) {
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");

		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		GLint scolor_loc = glGetUniformLocation(program, "scolor");
		vec3 sparkle_color = registry.sparkles.get(entity).colour;
		glUniform3f(scolor_loc, sparkle_color.x, sparkle_color.y, sparkle_color.z);

		gl_has_errors();

		GLint alpha_loc = glGetUniformLocation(program, "alpha");
		float sparkle_alpha = registry.sparkles.get(entity).life / 1000.f;
		glUniform1f(alpha_loc, sparkle_alpha);

		gl_has_errors();

		//glGenVertexArrays(1, &dummy_VAO);
		glBindVertexArray(dummy_VAO);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
			sizeof(TexturedVertex), (void*)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		glVertexAttribPointer(
			in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
			(void*)sizeof(
				vec3)); // note the stride to skip the preceeding vertex position

		// Enabling and binding texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();

		assert(registry.renderRequests.has(entity));
		GLuint texture_id =
			texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];

		glBindTexture(GL_TEXTURE_2D, texture_id);
		gl_has_errors();
	}
	else if (render_request.used_effect == EFFECT_ASSET_ID::BACKGROUND || render_request.used_effect == EFFECT_ASSET_ID::FOREGROUND ||
		render_request.used_effect == EFFECT_ASSET_ID::LIGHTS || render_request.used_effect == EFFECT_ASSET_ID::TEXTURED) {
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
		
		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		GLint player_pos_uloc = glGetUniformLocation(program, "player_pos");
		float rel_x = (registry.motions.get(registry.players.entities[0]).position.x - (window_width_px/2.f));

		glUniform1f(player_pos_uloc, rel_x);

		//glGenVertexArrays(1, &dummy_VAO);
		glBindVertexArray(dummy_VAO);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
			sizeof(TexturedVertex), (void*)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		glVertexAttribPointer(
			in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
			(void*)sizeof(
				vec3)); // note the stride to skip the preceeding vertex position

		// Enabling and binding texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();

		assert(registry.renderRequests.has(entity));
		GLuint texture_id =
			texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];

		glBindTexture(GL_TEXTURE_2D, texture_id);
		gl_has_errors();
	}
	else if (render_request.used_effect == EFFECT_ASSET_ID::BATTLE) {
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");

		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		//glGenVertexArrays(1, &dummy_VAO);
		glBindVertexArray(dummy_VAO);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
			sizeof(TexturedVertex), (void*)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		glVertexAttribPointer(
			in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
			(void*)sizeof(
				vec3)); // note the stride to skip the preceeding vertex position

		// Enabling and binding texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();

		assert(registry.renderRequests.has(entity));
		GLuint texture_id =
			texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];

		glBindTexture(GL_TEXTURE_2D, texture_id);
		gl_has_errors();

		// is the injury timer on
		GLint injured_uloc = glGetUniformLocation(program, "is_injured");
		GLfloat redness_uloc = glGetUniformLocation(program, "redness");
		GLuint counter_uloc = glGetUniformLocation(program, "counter");
		assert(counter_uloc >= 0);
		assert(injured_uloc >= 0);
		assert(redness_uloc >= 0);

		gl_has_errors();

		if (registry.injuryTimers.has(entity)) {
			InjuredTimer& injury = registry.injuryTimers.get(entity);
			glUniform1i(injured_uloc, 1);
			glUniform1f(redness_uloc, injury.redness_factor);
			glUniform1f(counter_uloc, (3000.f - injury.counter_ms) / 3000.f);
		}
		else {
			glUniform1i(injured_uloc, 0);
			glUniform1f(redness_uloc, 0.0);
		}

		gl_has_errors();

		
	}
	else if (render_request.used_effect == EFFECT_ASSET_ID::BATTLEBAR) {
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");

		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		//glGenVertexArrays(1, &dummy_VAO);
		glBindVertexArray(dummy_VAO);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
			sizeof(TexturedVertex), (void*)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		glVertexAttribPointer(
			in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
			(void*)sizeof(
				vec3)); // note the stride to skip the preceeding vertex position

		// Enabling and binding texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();

		assert(registry.renderRequests.has(entity));
		GLuint texture_id =
			texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];

		glBindTexture(GL_TEXTURE_2D, texture_id);
		gl_has_errors();

		// update percent_filled
		GLfloat percent_filled_uloc = glGetUniformLocation(program, "percent_filled");
		assert(percent_filled_uloc >= 0);

		gl_has_errors();

		glUniform1f(percent_filled_uloc, registry.healthBarFills.get(entity).percent_filled);

		gl_has_errors();


		}
	else
	{
		assert(false && "Type of render request not supported");
	}

	// Getting uniform locations for glUniform* calls
	GLint color_uloc = glGetUniformLocation(program, "fcolor");
	const vec3 color = registry.colors.has(entity) ? registry.colors.get(entity) : vec3(1);
	glUniform3fv(color_uloc, 1, (float *)&color);
	gl_has_errors();

	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);
	// GLsizei num_triangles = num_indices / 3;

	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
	// Setting uniform values to the currently bound program
	GLuint time_loc = glGetUniformLocation(currProgram, "time");
	glUniform1f(time_loc, (float)(glfwGetTime() * 10.0f));

	GLuint transform_loc = glGetUniformLocation(currProgram, "transform");
	glUniformMatrix3fv(transform_loc, 1, GL_FALSE, (float *)&transform.mat);
	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix3fv(projection_loc, 1, GL_FALSE, (float *)&projection);
	gl_has_errors();
	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();
}

// draw the intermediate texture to the screen, with some distortion to simulate
// wind
void RenderSystem::drawToScreen()
{

	glBindVertexArray(dummy_VAO);
	// Setting shaders
	// get the wind texture, sprite mesh, and program
	glUseProgram(effects[(GLuint)EFFECT_ASSET_ID::WIND]);
	gl_has_errors();
	// Clearing backbuffer
	int w, h;
	glfwGetFramebufferSize(window, &w, &h); // Note, this will be 2x the resolution given to glfwCreateWindow on retina displays
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, w, h);
	glDepthRange(0, 10);
	glClearColor(1.f, 0, 0, 1.0);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl_has_errors();
	// Enabling alpha channel for textures
	glDisable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// Draw the screen texture on the quad geometry
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[(GLuint)GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE]);
	glBindBuffer(
		GL_ELEMENT_ARRAY_BUFFER,
		index_buffers[(GLuint)GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE]); // Note, GL_ELEMENT_ARRAY_BUFFER associates
																	 // indices to the bound GL_ARRAY_BUFFER
	gl_has_errors();
	const GLuint wind_program = effects[(GLuint)EFFECT_ASSET_ID::WIND];
	// Set clock
	GLuint time_uloc = glGetUniformLocation(wind_program, "time");
	GLuint dead_timer_uloc = glGetUniformLocation(wind_program, "darken_screen_factor");
	glUniform1f(time_uloc, (float)(glfwGetTime() * 10.0f));
	ScreenState &screen = registry.screenStates.get(screen_state_entity);
	glUniform1f(dead_timer_uloc, screen.darken_screen_factor);
	gl_has_errors();
	// Set the vertex position and vertex texture coordinates (both stored in the
	// same VBO)
	GLint in_position_loc = glGetAttribLocation(wind_program, "in_position");
	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
	gl_has_errors();

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, off_screen_render_buffer_color);
	gl_has_errors();
	// Draw
	glDrawElements(
		GL_TRIANGLES, 3, GL_UNSIGNED_SHORT,
		nullptr); // one triangle = 3 vertices; nullptr indicates that there is
				  // no offset from the bound index buffer
	gl_has_errors();
}

// Render our game world
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
void RenderSystem::draw(StageSystem::Stage current_stage)
{
	// Getting size of window
	int w, h;
	glfwGetFramebufferSize(window, &w, &h); // Note, this will be 2x the resolution given to glfwCreateWindow on retina displays

	// First render to the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	gl_has_errors();
	// Clearing backbuffer
	glViewport(0, 0, w, h);
	glDepthRange(0.00001, 10);

	// Set color for background here
	switch (current_stage) {
	case StageSystem::Stage::main_menu:
		glClearColor(0.674, 0.847, 1.0, 1.0);
	case StageSystem::Stage::overworld:
		glClearColor(0.8549, 0.7765, 0.6941, 1.0); // light brown
	case StageSystem::Stage::cutscene:
		glClearColor(0.8549, 0.7765, 0.6941, 1.0); // light brown
	case StageSystem::Stage::turn_based:
		glClearColor(0, 0, 0, 1.0); // light brown
	case StageSystem::Stage::minigame:
		glClearColor(0.8549, 0.7765, 0.6941, 1.0); // light brown
	case StageSystem::Stage::cutscene_before:
		glClearColor(0.8549, 0.7765, 0.6941, 1.0); // light brown
	}

	glClearDepth(10.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST); // native OpenGL does not work with a depth buffer
							  // and alpha blending, one would have to sort
							  // sprites back to front
	gl_has_errors();
	mat3 projection_2D = createProjectionMatrix();
	glBindVertexArray(dummy_VAO);
	// Draw all textured meshes that have a position and size component
	for (Entity entity : registry.renderRequests.entities)
	{
		if (!registry.motions.has(entity))
			continue;
		// Note, its not very efficient to access elements indirectly via the entity
		// albeit iterating through all Sprites in sequence. A good point to optimize
		drawTexturedMesh(entity, projection_2D);
	}


	for (TextRenderRequest request : registry.textRenderRequests.components) {
		if (request.shown)
			renderText(request.text, request.position.x, request.position.y, request.scale, request.color, request.trans);
	}

	// Truely render to the screen
	drawToScreen();

	// flicker-free display with a double buffer
	glfwSwapBuffers(window);
	gl_has_errors();
}

void RenderSystem::renderText(const std::string& text, float x, float y,
		float scale, const glm::vec3& color,
		const glm::mat4& trans) {

		// activate the shaders!
		glUseProgram(m_font_shaderProgram);

		unsigned int textColor_location =
			glGetUniformLocation(
				m_font_shaderProgram,
				"textColor"
			);
		assert(textColor_location >= 0);
		glUniform3f(textColor_location, color.x, color.y, color.z);

		auto transform_location = glGetUniformLocation(
			m_font_shaderProgram,
			"transform"
		);
		assert(transform_location > -1);
		glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(trans));

		glBindVertexArray(m_font_VAO);

		// iterate through all characters
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			TextChar ch = m_ftCharacters[*c];

			float xpos = x + ch.Bearing.x * scale;
			float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;

			// update VBO for each character
			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};

			// render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);

			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, m_font_VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);

			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

mat3 RenderSystem::createProjectionMatrix()
{
	// Fake projection matrix, scales with respect to window coordinates
	float left = 0.f;
	float top = 0.f;

	gl_has_errors();
	float right = (float) window_width_px;
	float bottom = (float) window_height_px;

	float sx = 2.f / (right - left);
	float sy = 2.f / (top - bottom);
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	return {{sx, 0.f, 0.f}, {0.f, sy, 0.f}, {tx, ty, 1.f}};
}