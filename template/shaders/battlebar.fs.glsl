#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor;
uniform float percent_filled;

// Output color
layout(location = 0) out  vec4 color;


void main()
{
	if (texcoord.x > percent_filled) {
		color = vec4(vec3(1.0, 0.0, 0.0) + fcolor, 1.0) * texture(sampler0, vec2(texcoord.x, texcoord.y));
	}
	else {
		color = vec4(fcolor, 1.0) * texture(sampler0, vec2(texcoord.x, texcoord.y));
	}
	
}