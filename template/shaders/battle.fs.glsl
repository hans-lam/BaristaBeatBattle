#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor;
uniform int is_injured;
uniform float redness;

// Output color
layout(location = 0) out  vec4 color;


void main()
{
	if (is_injured == 1) {
		color = vec4(redness * vec3(1.0, 0.0, 0.0) + fcolor, 1.0) * texture(sampler0, vec2(texcoord.x, texcoord.y));
	}
	else {
		color = vec4(fcolor, 1.0) * texture(sampler0, vec2(texcoord.x, texcoord.y));
	}
	
}