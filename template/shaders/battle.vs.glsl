#version 330

// Input attributes
in vec3 in_position;
in vec2 in_texcoord;

// Passed to fragment shader
out vec2 texcoord;

// Application data
uniform mat3 transform;
uniform mat3 projection;
uniform float counter;
uniform int is_injured;

void main()
{
	texcoord = in_texcoord;
	float dx;
	if (is_injured == 1) {
	// hardcoded parameters - thank u desmos
		dx = 0.80 * exp(-4.0 * counter) * sin(45.0 * counter);
	} else {
		dx = 0.0;
	}

	vec3 pos = projection * transform * vec3(in_position.x + dx, in_position.y, 1.0);
	gl_Position = vec4(pos.xy, in_position.z, 1.0);

	
}