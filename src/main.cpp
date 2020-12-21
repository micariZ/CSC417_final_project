#include "main.h"
// This code is based on: https://github.com/kodai100/OpenGL_MPMSnowSimulation2D

void start(string scene_id) {
	Scene* scene;
	if ((scene_id.compare("scene1")) == 0) {
		scene = Scene::snowman();
	}
	else if ((scene_id.compare("scene2")) == 0) {
		scene = Scene::snowballSmash();
	}
	else {
		scene = Scene::highspeedSnowballSmash();
	}

	snow_simulator = new SnowSimulation(scene);
}

void update() {
	 snow_simulator->update();
}

void render() {
	snow_simulator->snow->draw();
	snow_simulator->grid->draw();
}


// -------------- GLFW Routine ------------------------

int main(int argc, char** argv) {
	if (!glfwInit())
		return - 1;

	if (argc != 2) {
		std::cout << "This problem only excepts one argument. You can choose from \n scene1\n scene2\n scene3";
		return -1;
	}
		
	GLFWwindow* window = initGLFWContext();

	srand(time(NULL));

	initGLContext();

	start(argv[1]);

	frame_count = 0;
	while (!glfwWindowShouldClose(window)) {

		update();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		render();

		glfwSwapBuffers(window);
		glfwPollEvents();

		frame_count++;
	}

	//Exit
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}


void initGLContext() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, WIN_SIZE_X, WIN_SIZE_Y);
	glOrtho(0, WIN_METERS_X, 0, WIN_METERS_Y, 0, 1);
}


GLFWwindow* initGLFWContext() {
	glfwSetErrorCallback(error_callback);

	GLFWwindow* window = glfwCreateWindow(WIN_SIZE_X, WIN_SIZE_Y, "Snow Simulator", NULL, NULL);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	return window;
}

//Print all errors to console
static void error_callback(int error, const char* description) {
	printf("\nError: %s", description);
}
