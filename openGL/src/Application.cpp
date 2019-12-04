#include"GL/glew.h"
#include "GLFW/glfw3.h"
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<array>
#include<tuple>
struct ShaderProgramSource {
	std::string vertexShader;
	std::string fragmentShader;
};
static ShaderProgramSource ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);
	enum class typeShader {
		ERROR = -1, VERTEX = 0, FRAGMENT
	};

	std::string line;
	std::stringstream ss[2]; 
	typeShader type(typeShader::ERROR);
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) 
		{	
			if (line.find("vertex") != std::string::npos)
				type = typeShader::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = typeShader::FRAGMENT;
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	std::string vs = ss[0].str();
	std::string fs = ss[1].str();
	//return std::make_pair(vs, fs);
	return { vs, fs };
	/*return { ss[0].str(), ss[1].str() };*/
	
}


static unsigned int CompileShader( unsigned int type, const std::string& source ) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	int resault; 
	glGetShaderiv(id, GL_COMPILE_STATUS, &resault);
	if (resault == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "fail to compile"<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!"<< std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	

	return id;
}

static unsigned int  CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	return program;
}


int main(void)
{
	
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}


	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	
	if (GLEW_OK != glewInit())
		std::cout << "error" << std::endl;
	
	std::cout << glGetString(GL_VERSION) << std::endl;
	
	float position[6] = {
		-0.5f,-0.5f,
		 0.0f, 0.5f,
		 0.5f,-0.5f

	};

	unsigned int buffer;
	glCreateBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
	

	auto source =  ParseShader("res/shaders/Basic.shader");

	unsigned int shader = CreateShader(source.vertexShader, source.fragmentShader);
	glUseProgram(shader);
	/*std::cout << "Vertex123" << std::endl;
	std::cout << source.vertexShader << std::endl;
	std::cout << "Fragment123" << std::endl;
	std::cout << source.fragmentShader << std::endl;*/
	
	

		/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}