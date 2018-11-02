#include "Shader.h"


Shader::Shader(bool showCompileInfo) {
	verbose = showCompileInfo;
	init();
}

Shader::Shader() {
	verbose = DEFAULT_VERBOSE;
	init();
}


Shader::~Shader() {
	shaders.clear();
	shadersTypes.clear();
}


bool Shader::attachShader(shaderType type, std::string sourceCodeOrFile, int mode) {

	std::string sourceCode;

	if (mode == SHADER_FILE)
		sourceCode = loadShaderFile(sourceCodeOrFile);
	else if (mode == SHADER_CODE)
		sourceCode = sourceCodeOrFile;
	else
		return false;

	if (sourceCode.empty())
		return false;

	const GLchar* source = sourceCode.c_str();

	GLuint shader = ge::gl::glCreateShader(shadersTypes.at(type));
	ge::gl::glShaderSource(shader, 1, &source, NULL);
	ge::gl::glCompileShader(shader);
	shaders.at(type) = shader;

	GLint result;
	ge::gl::glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (!result) {

		if (verbose) {
			GLint logSize;
			ge::gl::glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
			GLchar* infoLog = new GLchar[logSize];

			ge::gl::glGetShaderInfoLog(shader, logSize, NULL, infoLog);
			std::cout << infoLog << std::endl;

			delete[] infoLog;
		}

		return false;
	}

	return true;
}


bool Shader::attachShader(shaderType type, std::string sourceCodeOrFile) {

	if (sourceCodeOrFile.find("void main()") != std::string::npos)
		return attachShader(type, sourceCodeOrFile, SHADER_CODE);
	else
		return attachShader(type, sourceCodeOrFile, SHADER_FILE);
}


bool Shader::linkProgram() {

	program = ge::gl::glCreateProgram();

	for (int i = VERTEX; i <= COMPUTE; i++) {
		if (shaders.at(i))
			ge::gl::glAttachShader(program, shaders.at(i));

	}

	ge::gl::glLinkProgram(program);

	GLint result;
	ge::gl::glGetProgramiv(program, GL_LINK_STATUS, &result);

	for (int i = VERTEX; i <= COMPUTE; i++)
		ge::gl::glDeleteShader(shaders.at(i));


	if (!result) {

		if (verbose) {
			GLint logSize;
			ge::gl::glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
			GLchar* infoLog = new GLchar[logSize];


			ge::gl::glGetShaderInfoLog(program, logSize, NULL, infoLog);
			std::cout << infoLog << std::endl;

			delete[] infoLog;
		}

		return false;
	}

	return true;
}


GLuint Shader::getProgram() {
	return program;
}


void Shader::useProgram() {
	ge::gl::glUseProgram(program);
}


std::string Shader::loadShaderFile(std::string pathToFile) {

	try {

		std::ifstream shaderFile;
		shaderFile.exceptions(std::ifstream::badbit);
		shaderFile.open(pathToFile);

		std::stringstream stream;
		stream << shaderFile.rdbuf();
		shaderFile.close();

		std::string code = stream.str();

		return code;
	}

	catch (std::ifstream::failure e) {

		std::cout << "Shader loading error" << std::endl;
		return "";

	}

	return "";

}

void Shader::init() {
	program = 0;

	for (int i = VERTEX; i <= COMPUTE; i++)
		shaders.push_back(0);

	shadersTypes.push_back(GL_VERTEX_SHADER);
	shadersTypes.push_back(GL_GEOMETRY_SHADER);
	shadersTypes.push_back(GL_TESS_CONTROL_SHADER);
	shadersTypes.push_back(GL_TESS_EVALUATION_SHADER);
	shadersTypes.push_back(GL_FRAGMENT_SHADER);
	shadersTypes.push_back(GL_COMPUTE_SHADER);
}