#include "ShaderManager.h"

bool ShaderManager::_instanceFlag = false;
ShaderManager* ShaderManager::single = NULL;
ShaderManager* ShaderManager::GetInstance()
{
	if (!_instanceFlag)
	{
		single = new ShaderManager();
		_instanceFlag = true;
		return single;
	}
	else
	{
		return single;
	}
}
ShaderManager::ShaderManager()
{
	this->resources.textures[0] = this->MakeTexture("image/1.png");
	this->resources.textures[1] = this->MakeTexture("image/2.png");
	this->resources.textures[2] = this->MakeTexture("image/3.png");
	this->resources.textures[3] = this->MakeTexture("image/4.png");
	this->resources.textures[4] = this->MakeTexture("image/5.png");
	this->resources.textures[5] = this->MakeTexture("image/6.png");
	this->resources.vertexShader = this->MakeShader(GL_VERTEX_SHADER, "VertexShader.vert");
	this->resources.fragmentShader = this->MakeShader(GL_FRAGMENT_SHADER, "FragmentShader.frag");
	this->resources.shaderProgram = this->MakeProgram(resources.vertexShader, resources.fragmentShader);
	this->DefineAttribs();
	this->DefineUniforms();
	this->DefineOuts();
	

	// Combining shaders into program
//	GLuint shaderProgram = glCreateProgram();
//	glAttachShader(shaderProgram, vertexShader);
//	glAttachShader(shaderProgram, fragmentShader);
//	glBindFragDataLocation(shaderProgram, 0, "outColor");	// for fragmentShader apply to multiple buffers
//	glLinkProgram(shaderProgram);
//	glUseProgram(shaderProgram);
}
ShaderManager::~ShaderManager()
{
	this->_instanceFlag = false;

	glDeleteTextures(6, resources.textures);
	glDeleteProgram(resources.shaderProgram);
	glDeleteShader(resources.vertexShader);
	glDeleteShader(resources.fragmentShader);
}

// Public Methods other than Singleton handler
void ShaderManager::ActivateProgram()
{
	glUseProgram(resources.shaderProgram);
}

void ShaderManager::UseTexture(GLuint texture)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	resources.uniforms.texture = glGetUniformLocation(resources.shaderProgram, "uniTexture");
	glUniform1i(resources.uniforms.texture, 0);
}

// Private Methods
GLuint ShaderManager::MakeShader(GLenum type, const char *filename)
{
	GLint length;
	GLchar *source = (GLchar*)this->FileContents(filename, &length);
	GLuint shader;
	GLint shader_ok;

	if (!source)
		return 0;

	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar**)&source, &length);
	free(source);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
	if (!shader_ok) {
		fprintf(stderr, "Failed to compile %s:\n", filename);
		this->ShowInfoLog(shader, glGetShaderiv, glGetShaderInfoLog);
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

GLuint ShaderManager::MakeProgram(GLuint vertex_shader, GLuint fragment_shader)
{
	GLint program_ok;

	GLuint program = glCreateProgram();

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
	if (!program_ok) {
		fprintf(stderr, "Failed to link shader program:\n");
		this->ShowInfoLog(program, glGetProgramiv, glGetProgramInfoLog);
		glDeleteProgram(program);
		return 0;
	}
	return program;
}

GLuint ShaderManager::MakeTexture(const char *filename)
{
	int width, height;
	unsigned char* image;
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}

void ShaderManager::DefineAttribs()
{
	resources.attribs.position = glGetAttribLocation(resources.shaderProgram, "position");
	glEnableVertexAttribArray(resources.attribs.position);
	glVertexAttribPointer(resources.attribs.position, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	resources.attribs.color = glGetAttribLocation(resources.shaderProgram, "color");
	glEnableVertexAttribArray(resources.attribs.color);
	glVertexAttribPointer(resources.attribs.color, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	resources.attribs.texcoord = glGetAttribLocation(resources.shaderProgram, "texcoord");
	glEnableVertexAttribArray(resources.attribs.texcoord);
	glVertexAttribPointer(resources.attribs.texcoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
}

void ShaderManager::DefineUniforms()
{
	//resources.uniforms.model	= glGetUniformLocation(resources.shaderProgram, "model");
	//resources.uniforms.view		= glGetUniformLocation(resources.shaderProgram, "view");
	//resources.uniforms.proj		= glGetUniformLocation(resources.shaderProgram, "proj");
}

void ShaderManager::DefineOuts()
{
	glBindFragDataLocation(resources.shaderProgram, 0, "outColor");
}

void* ShaderManager::FileContents(const char* filename, GLint* length)
{
	FILE *f = fopen(filename, "r");
	void *buffer;

	if (!f) {
		fprintf(stderr, "Unable to open %s for reading\n", filename);
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	*length = ftell(f);
	fseek(f, 0, SEEK_SET);

	buffer = malloc(*length + 1);
	*length = fread(buffer, 1, *length, f);
	fclose(f);
	((char*)buffer)[*length] = '\0';

	return buffer;
}

void ShaderManager::ShowInfoLog(
	GLuint object,
	PFNGLGETSHADERIVPROC glGet__iv,
	PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
	GLint log_length;
	char *log;

	glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
	log = (char*)malloc(log_length);
	glGet__InfoLog(object, log_length, NULL, log);
	fprintf(stderr, "%s", log);
	free(log);
}