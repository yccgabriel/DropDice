#version 150 core

//uniform vec3 triangleColor;
in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D	texKitten;
uniform sampler2D	texPuppy;
uniform float		blendRatio;

void main()
{
	//outColor = vec4(triangleColor, 1.0);
	//outColor = texture(tex, Texcoord) * vec4(Color, 1.0);
	vec4 colKitten = texture(texKitten, Texcoord);
	vec4 colPuppy = texture(texPuppy, Texcoord);
	outColor = mix(colKitten, colPuppy, blendRatio);
}