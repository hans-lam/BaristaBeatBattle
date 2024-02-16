#version 330

// Input attributes
in vec3 in_position;
in vec2 in_texcoord;

// Passed to fragment shader
out vec2 texcoord;

// Application data
uniform mat3 transform;
uniform mat3 projection;
uniform float time;

void main()
{
	texcoord = in_texcoord;

    float displacement = 0.3 * sin(time * 0.5);
    
    vec3 displacedPosition = in_position + vec3(0.0, displacement, 0.0);
	vec3 pos = projection * transform * vec3(displacedPosition.xy, 1.0);
	gl_Position = vec4(pos.xy, in_position.z, 1.0);
}