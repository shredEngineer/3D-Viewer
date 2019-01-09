#include "Main.hpp"

//	Data mode setup
int dataMode = 1;

//	Camera setup
GLfloat cameraScale = 1.0, cameraAngleX = 20.0, cameraAngleY = 70.0;

//	Application main
int main(int argc, char **argv)
{
	const char *title = "3D-Viewer (C) 2019 Paul Wilhelm <anfrage@paulwilhelm.de>";

	//	Set console title
	SetConsoleTitle(title);

	//	Show header
	std::cout << title << std::endl;
	std::cout << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << "\tQuit:\t<Escape>" << std::endl;
	std::cout << "\tMove:\t<Arrows>          or mouse drag" << std::endl;
	std::cout << "\tZoom:\t<PageUp>/<PageDn> or mouse scoll" << std::endl;
	std::cout << "\tMode:\t<+>/<->" << std::endl;
	std::cout << std::endl;

	//	Setup GLFW / OpenGL
	if (!glfwInit())
	{
		std::cerr << "ERROR: glfwInit failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	//	Enable multisampling
	glfwWindowHint(GLFW_SAMPLES, 8);

	//	Create window
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, title, NULL, NULL);
	if (!window)
	{
		std::cerr << "ERROR: glfwCreateWindow failed" << std::endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	//	Set window icon
	SendMessage(
		glfwGetWin32Window(window), WM_SETICON, ICON_SMALL,
		(LPARAM) LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(0))
	);

	//	Register & call window resize handler
	glfwSetFramebufferSizeCallback(window, windowResizeCallback);
	windowResizeCallback(window, SCREEN_WIDTH, SCREEN_HEIGHT);

	//	Setup Z-buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//	Setup multisampling
	glEnable(GL_MULTISAMPLE);

	//	Setup transparency
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//	Setup keyboard mode / register character event handler
	glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);
	glfwSetCharCallback(window, keyboardCharacterCallback);

	//	Register mouse scroll event handler
	glfwSetScrollCallback(window, mouseScrollCallback);

	//	Calculate optimal cube size
	float cubeSize = 0.5 / std::max(std::max(DATA_SIZE_X, DATA_SIZE_Y), DATA_SIZE_Z);

	//	Superloop
	while (!glfwWindowShouldClose(window))
	{
		//	Clear screen / Z-buffer
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	Camera transform
		if (cameraScale < 0.1) cameraScale = 0.1;
		if (cameraScale > 1.0) cameraScale = 1.0;
		glMatrixMode(GL_MODELVIEW_MATRIX);
		glLoadIdentity();
		glScalef(cameraScale, cameraScale, cameraScale);
		glRotatef(cameraAngleX, 0, 1, 0);
		glRotatef(cameraAngleY, 1, 0, 0);

		//	------------------------------------------------------------

		//	Enable vertex array usage
		glEnableClientState(GL_VERTEX_ARRAY);

		//	Draw container wireframe cube
		glPushMatrix();
		glScalef(DATA_SIZE_X * cubeSize, DATA_SIZE_Y * cubeSize, DATA_SIZE_Z * cubeSize);
		glColor4f(1.0, 1.0, 1.0, 0.5);
		glLineWidth(1.0);
		glVertexPointer(3, GL_FLOAT, 0, wireframe_cube);
		glDrawArrays(GL_LINES, 0, 24);
		glPopMatrix();

		//	Center figure
		glTranslatef((1 - DATA_SIZE_X) * cubeSize, (1 - DATA_SIZE_Y) * cubeSize, (1 - DATA_SIZE_Z) * cubeSize);

		//	Refresh data matrix
		static int dataTime = 0;
		refreshData(dataMode, dataTime++);

		//	Draw non-zero data points
		for (int z = 0; z < DATA_SIZE_Z; ++z)
		{
			for (int y = 0; y < DATA_SIZE_Y; ++y)
			{
				for (int x = 0; x < DATA_SIZE_X; ++x)
				{
					if (getData(x, y, z) != 0)
					{
						//	Save state
						glPushMatrix();

						//	Position / scale
						glTranslated(x * cubeSize * 2, y * cubeSize * 2, z * cubeSize * 2);
						glScalef(cubeSize, cubeSize, cubeSize);

						//	Convert data value to color
						rainbowColor(getData(x, y, z), 1.0);

						//	Draw colored solid cube
						glVertexPointer(3, GL_FLOAT, 0, solid_cube);
						glDrawArrays(GL_QUADS, 0, 24);

						//	Draw wireframe cube
						glColor4f(1.0, 1.0, 1.0, 0.75);
						glVertexPointer(3, GL_FLOAT, 0, wireframe_cube);
						glDrawArrays(GL_LINES, 0, 24);

						//	Restore state
						glPopMatrix();
					}
				}
			}
		}

		//	Disable vertex array usage
		glDisableClientState(GL_VERTEX_ARRAY);

		//	------------------------------------------------------------

		//	Update screen
		glfwSwapBuffers(window);

		//	Get user events
		glfwPollEvents();

		//	Handle mouse drag
		static int mousePrevX = -1, mousePrevY = -1;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			double mouseX, mouseY;
			glfwGetCursorPos(window, &mouseX, &mouseY);
			if (mousePrevX == -1) mousePrevX = mouseX;
			if (mousePrevY == -1) mousePrevY = mouseY;
			int diffX = mousePrevX - mouseX;
			int diffY = mousePrevY - mouseY;
			float length = sqrt(diffX * diffX + diffY * diffY);
			if (length != 0)
			{
				cameraAngleX += diffX / length * ROTATE_SPEED;
				cameraAngleY += diffY / length * ROTATE_SPEED;
			}
			mousePrevX = mouseX;
			mousePrevY = mouseY;
		}
		else
		{
			mousePrevX = -1;
			mousePrevY = -1;
		}

		//	Handle keypress
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) cameraAngleX += ROTATE_SPEED;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) cameraAngleX -= ROTATE_SPEED;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) cameraAngleY += ROTATE_SPEED;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) cameraAngleY -= ROTATE_SPEED;
		if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) cameraScale += ZOOM_SPEED;
		if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) cameraScale -= ZOOM_SPEED;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) break;
	}

	//	Quit application
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

//	Window resize callback
void windowResizeCallback(GLFWwindow* window, int width, int height)
{
	//	Setup square viewport
	if (width > height)
	{
		glViewport(0, (height - width) / 2, width, width);
	}
	else
	{
		glViewport((width - height) / 2, 0, height, height);
	}

	//	Setup orthogonal projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, width, height, 0.0f, 0.0f, 1.0f);
}

//	Keyboard character event callback
void keyboardCharacterCallback(GLFWwindow* window, unsigned int code)
{
	if (code == '+') dataMode++;
	if (code == '-') dataMode--;
	if (dataMode < 0) dataMode = 0;
	std::cout << "dataMode = " << dataMode << std::endl;
}

//	Mouse scoll event callback
void mouseScrollCallback(GLFWwindow* window, double offsetX, double offsetY)
{
	cameraScale += offsetY * ZOOM_SPEED;
}

//	Set color
void rainbowColor(float hue, float alpha)
{
	hue += 1 / 6.0;
	if (hue > 1) hue -= 1.0;
	float r = 0, g = 0, b = 0;
	float x = hue * 6;
	if     ((x >= 0 && x <= 1) || (x >= 5 && x <= 6)) r = 1;
	else if (x >= 4 && x <= 5) r = x - 4;
	else if (x >= 1 && x <= 2) r = 2 - x;
	if      (x >= 1 && x <= 3) g = 1;
	else if (x >= 0 && x <= 1) g = x - 0;
	else if (x >= 3 && x <= 4) g = 4 - x;
	if      (x >= 3 && x <= 5) b = 1;
	else if (x >= 2 && x <= 3) b = x - 2;
	else if (x >= 5 && x <= 6) b = 6 - x;
	glColor4f(r, g, b, alpha);
}
