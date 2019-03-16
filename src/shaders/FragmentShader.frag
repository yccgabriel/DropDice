#version 150 core

//uniform vec3 triangleColor;
in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

//uniform sampler2D	texKitten;
//uniform sampler2D	texPuppy;
uniform bool		useTexture = true;
uniform sampler2D	uniTexture;
uniform float		blendRatio;
uniform float		distortion;

void main()
{
			//outColor = vec4(triangleColor, 1.0);
			//outColor = texture(tex, Texcoord) * vec4(Color, 1.0);
	vec2 texcoord = Texcoord;
			//if(texcoord.y > 0.5)		texcoord.y = 1-texcoord.y;
			//vec4 colKitten = texture(textures[0], texcoord);
			//vec4 colPuppy = texture(textures[1], Texcoord);
			//outColor = mix(colKitten, colPuppy, blendRatio);
	
			//vec4 colKitten, colPuppy;
			//if(Texcoord.y < 0.5)
			//	colKitten = texture(textures[0], Texcoord);
			//else
			//	colKitten = texture(textures[0], vec2(Texcoord.x + sin(Texcoord.y*50.0f)*0.05, 1-Texcoord.y));
			//colPuppy = texture(textures[1], Texcoord);
			//vec4 texColor = mix(colKitten, colPuppy, blendRatio);
			//outColor = vec4(Color, 1.0) * texColor;
	if (useTexture)
		outColor = vec4(Color, 1.0) * texture(uniTexture, Texcoord);
	else
		outColor = vec4(Color, 1.0);
}