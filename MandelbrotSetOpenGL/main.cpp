#include "IndexBuffer.h"
#include "VertexBuffer.h"

void framebuffer_size_callback(GLFWwindow*, int, int);
void process_input(GLFWwindow*);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int num_frames = 0;
float last_time = 0.0f;

double zoom = 0.0;
double offset_x = 0.0;
double offset_y = 0.0;

const double delta = 0.03;

void countFPS()
{
	double current_time = glfwGetTime();
	num_frames++;
	if (current_time - last_time >= 1.0)
	{
		std::cout << 1000.0 / num_frames << "ms / frame\n";
		num_frames = 0;
		last_time += 1.0;
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for MAC OS X
#endif // __APPLE__

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Manderlbrot", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load GLAD" << std::endl;
		return -1;
	}

	// Shader shit
	Shader shader("vertex.glsl", "fragment.glsl");
	{
		double vertices[] = {
			-1.0f, -1.0f,
			 1.0f, -1.0f,
			 1.0f,  1.0f,

			 1.0f,  1.0f,
			-1.0f,  1.0f,
			-1.0f, -1.0f
		};

		unsigned int VAO;
		VertexBuffer VB(&vertices, sizeof(vertices));

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), (void*)0);
		glEnableVertexAttribArray(0);
		
		shader.use();
		shader.setInt("maxIterations", 200);
		shader.setDouble("zoom", pow(2, zoom));
		glUniform2d(glGetUniformLocation(shader.ID, "offset"), 0.0, 0.0);
		while (!glfwWindowShouldClose(window))
		{
			process_input(window);

			

			glClear(GL_COLOR_BUFFER_BIT);

			countFPS();
			//std::cout << zoom << std::endl;
		
			shader.use();
			shader.setDouble("zoom", pow(2, zoom));
			glUniform2d(glGetUniformLocation(shader.ID, "offset"), offset_x, offset_y);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		glDeleteVertexArrays(1, &VAO);
	}

	glfwTerminate();
	return 0;
}


int WinMain() {
	main();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		offset_y -= (delta / pow(2, zoom));
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		offset_y += (delta / pow(2, zoom));
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		offset_x += (delta / pow(2, zoom));
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		offset_x -= (delta / pow(2, zoom));
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
		zoom += 2 * delta;
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
		zoom -= 2 * delta;
}