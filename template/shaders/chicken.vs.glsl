#version 330

// Input attributes
in vec3 in_position;
in vec3 in_color;

out vec3 vcolor;
out vec2 vpos;
out vec2 tex_coords;

// Application data
uniform mat3 transform;
uniform mat3 projection;
uniform float time;
uniform int curr_frame;

const float total_frames = 10.0;
const float sprite_width = 1.0/total_frames;

void main()
{
	float oscillate;
	
	// check if walking
	if (curr_frame == 2 || curr_frame == 4 || curr_frame == 6 || curr_frame == 8) {
		oscillate = sin(time);
	} else {
		oscillate = 1.0;
	}

	// get current sprite sheet frame offset
	float dx;
	if (oscillate > 0) {
		dx = curr_frame * sprite_width;
	} else {
		dx = (curr_frame + 1) * sprite_width;
	}


	// get tecture coordinates
	
	tex_coords = vec2((in_position.x + 0.5), (in_position.y + 0.5)); // scale [0,1]
    tex_coords.x = (tex_coords.x * sprite_width + dx);

	vpos = in_position.xy; // local coordinated before transform

	vcolor = in_color * 3.0;

	vec3 pos = projection * transform * vec3(in_position.xy, 1.0); // why not simply *in_position.xyz ?
	gl_Position = vec4(pos.xy, in_position.z, 1.0);
}