#version 330

// From vertex shader
in vec2 texcoord;

// Application data
uniform sampler2D sampler0;
uniform vec3 fcolor;
uniform vec3 scolor;
uniform float alpha;

// Output color
layout(location = 0) out  vec4 color;

void main()
{
	color = vec4(fcolor * scolor, alpha) * texture(sampler0, vec2(texcoord.x, texcoord.y));
}
