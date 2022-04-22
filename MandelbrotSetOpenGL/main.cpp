#include "headers.h"

void framebuffer_size_callback(GLFWwindow*, int, int);
void mouseCallback(GLFWwindow*, double, double);
void process_input(GLFWwindow*);
void dT();
void countFPS(std::string& outStr);
void cnum(std::string& cnum, double, double);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;
bool mouseF = true;

int num_frames = 0;
int maxIters = 128;
double last_time = 0.0;
double lastFrame = 0.0;
double currentFrame = 0.0;

double delta = 0.0;

double zoom = 0.0;
double zoom_speed = 10.0;

double offset_x = 0.0;
double offset_y = 0.0;
double jnum_x = 0.0;
double jnum_y = 0.0;

std::string frametime;
std::string complex_num = "0.000000+0.000000i";
std::string cursor_pos = "0.000000+0.000000i";

// from 0 to 3 currently
// 0 - Burning Ship fractal
// 1 - Newtons fractal
// 2 - Julia fractal of Mandelbrot fractal
// 3 - Mandelbrot fractal
int fractalType = 2;
int coloringType = 0; // from 0 to 1 currently

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
	glfwSetCursorPosCallback(window, mouseCallback);
	//glfwSetCursorPos(window, lastX, lastY);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load GLAD" << std::endl;
		return -1;
	}

	Shader shader("vertex.glsl", "fragment.glsl");
	double vertices[] = {
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f,

		 1.0f,  1.0f,
		-1.0f,  1.0f,
		-1.0f, -1.0f
	};

	unsigned int VAO, VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), (void*)0);
	glEnableVertexAttribArray(0);
		
	shader.use();
	shader.setInt("maxIterations", maxIters);
	shader.setDouble("zoom", pow(2, zoom));
	glUniform2d(glGetUniformLocation(shader.ID, "offset"), offset_x, offset_y);
	shader.setInt("coloringType", coloringType);
	shader.setInt("fractalType", fractalType);
		
	while (!glfwWindowShouldClose(window))
	{
		process_input(window);
		glClear(GL_COLOR_BUFFER_BIT);

		dT();
		countFPS(frametime);
		cnum(complex_num, offset_x, offset_y);
		cnum(cursor_pos, 
			(2.0 * lastX / double(SCR_WIDTH) - 1.0) / pow(2, zoom) - offset_x, 
			(2.0 * lastY / double(SCR_HEIGHT) - 1.0) / pow(2, zoom) - offset_y);
		glfwSetWindowTitle(window, ("Fractal / " + frametime + " / " + complex_num + " / " + cursor_pos).c_str());
		
		double time = glfwGetTime() / 4.0;		

		shader.use();
		shader.setDouble("zoom", pow(2, zoom));
		glUniform2d(glGetUniformLocation(shader.ID, "offset"), offset_x, offset_y);
		glUniform2d(glGetUniformLocation(shader.ID, "jnum"), jnum_x, jnum_y);
		//glUniform2d(glGetUniformLocation(shader.ID, "jnum"), 0.7885 * cos(time), 0.7885 * sin(time));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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
		zoom += zoom_speed * delta;
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
		zoom -= zoom_speed * delta;

	
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (mouseF)
	{
		lastX = xpos;
		lastY = ypos;
		mouseF = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		offset_x += xoffset / (double(SCR_WIDTH) * pow(2, zoom)) * 2.0;
		offset_y += yoffset / (double(SCR_HEIGHT) * pow(2, zoom)) * 2.0;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		jnum_x = (2.0 * lastX / double(SCR_WIDTH) - 1.0) / pow(2, zoom) - offset_x;
		jnum_y = - (2.0 * lastY / double(SCR_HEIGHT) - 1.0) / pow(2, zoom) - offset_y;
	}

	lastX = xpos;
	lastY = ypos;
}

void dT()
{
	currentFrame = glfwGetTime();
	delta = (currentFrame - lastFrame) / 2;
	lastFrame = currentFrame;
}

void countFPS(std::string& outStr)
{
	num_frames++;
	double dt = currentFrame - last_time;
	if (dt >= 1.0)
	{
		outStr =
			std::to_string(1000.0 / num_frames) + " ms - frame / " +
			std::to_string(int(num_frames / (dt))) + " fps";
		num_frames = 0;
		last_time += 1.0;
	}
}

void cnum(std::string &cnum, double x, double y)
{
	if (-y > 0)
		cnum = std::to_string(-x) + "+" + std::to_string(-y) + "i";
	else if (-y < 0)
		cnum = std::to_string(-x) + std::to_string(-y) + "i";
}