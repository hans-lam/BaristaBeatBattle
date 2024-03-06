#version 330

// From Vertex Shader
in vec3 vcolor;
in vec2 vpos; // Distance from local origin
in vec2 tex_coords;

// Application data
uniform sampler2D sprite_sheet;
uniform vec3 fcolor;

// Output color
layout(location = 0) out vec4 color;

void main()
{
	//color = vec4(fcolor * vcolor, 1.0);
	
	color = texture(sprite_sheet, tex_coords);
	color.xyz *= vcolor;
	color.xyz /= vcolor;

}

