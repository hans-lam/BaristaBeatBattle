#version 330

// From Vertex Shader
in vec3 vcolor;
in vec2 vpos; // Distance from local origin
in vec2 tex_coords;

// Application data
uniform sampler2D sprite_sheet;
uniform vec3 fcolor;
uniform int is_injured;
uniform float redness;

// Output color
layout(location = 0) out vec4 color;

void main()
{
	//color = vec4(fcolor * vcolor, 1.0);
	
	//color = texture(sprite_sheet, tex_coords);

	if (is_injured == 1) {
		color = vec4(redness * fcolor * vcolor * vec3(1.0, 0.0, 0.0), 1.0) * texture(sprite_sheet, tex_coords);
	}
	else {
		color = texture(sprite_sheet, tex_coords);
	}

	color.xyz *= vcolor;
	color.xyz /= vcolor;

}