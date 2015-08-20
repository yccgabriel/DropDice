#version 150 core

//uniform vec3 triangleColor;
in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D	texKitten;
uniform sampler2D	texPuppy;
uniform float		blendRatio;
uniform float		distortion;

void main()
{
	//outColor = vec4(triangleColor, 1.0);
	//outColor = texture(tex, Texcoord) * vec4(Color, 1.0);
	vec2 texcoord = Texcoord;
	//if(texcoord.y > 0.5)		texcoord.y = 1-texcoord.y;
	//vec4 colKitten = texture(texKitten, texcoord);
	//vec4 colPuppy = texture(texPuppy, Texcoord);
	//outColor = mix(colKitten, colPuppy, blendRatio);
	
	vec4 colKitten, colPuppy;
	if(Texcoord.y < 0.5)
		colKitten = texture(texKitten, Texcoord);
	else
		colKitten = texture(texKitten, vec2(Texcoord.x + sin(Texcoord.y*50.0f)*0.05, 1-Texcoord.y));
	colPuppy = texture(texPuppy, Texcoord);
	vec4 texColor = mix(colKitten, colPuppy, blendRatio);
	outColor = vec4(Color, 1.0) * texColor;
}