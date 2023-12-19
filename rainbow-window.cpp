#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Function to convert from HSV to RGB
void HSVtoRGB(float H, float S, float V, float& R, float& G, float& B) {
	if (S == 0.0) {
		// If it's grayscale, then R, G and B are all the same and equal to V
		R = G = B = V;
	}
	else {
		if (H == 360) {
			H = 0;
		}
		else {
			H = H / 60;
		}
		int i = static_cast<int>(trunc(H));
		float f = H - i;
		float p = V * (1 - S);
		float q = V * (1 - (S * f));
		float t = V * (1 - (S * (1 - f)));
		switch (i) {
		case 0:
			R = V;
			G = t;
			B = p;
			break;
		case 1:
			R = q;
			G = V;
			B = p;
			break;
		case 2:
			R = p;
			G = V;
			B = t;
			break;
		case 3:
			R = p;
			G = q;
			B = V;
			break;
		case 4:
			R = t;
			G = p;
			B = V;
			break;
		default: // case 5:
			R = V;
			G = p;
			B = q;
			break;
		}
	}
}

int main() {
	// Initialize the GLFW library
	glfwInit();

	// Set the GLFW to use OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable the CORE profile mode in GLFW to use only modern OpenGL features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFW window with dimensions 800x800 pixels and title "OpenGL"
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
	// Check if the window creation was successful
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Make the created window the current context for OpenGL commands
	glfwMakeContextCurrent(window);

	// Load GLAD to manage OpenGL function pointers
	gladLoadGL();

	// Define the viewport dimensions within the window
	// Here, it covers the entire window from (0,0) to (800, 800)
	glViewport(0, 0, 800, 800);

	// Set the saturation and value constants for vivid color
	const float saturation = 1.0f;
	const float value = 1.0f;
	float hue = 0.0f; // Hue angle will vary from 0 to 360
	float red, green, blue; // RGB components

	// Main while loop
	while (!glfwWindowShouldClose(window)) {
		// Update the hue
		hue += 0.005f; // Speed of hue change
		if (hue > 360.0f) hue = 0.0f; // Loop back to start if over 360

		// Convert HSV to RGB
		HSVtoRGB(hue, saturation, value, red, green, blue);

		// Set the new background color
		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);

		// Handle GLFW events
		glfwPollEvents();
	}

	// Clean up: Destroy the window to free resources
	glfwDestroyWindow(window);
	// Terminate GLFW to clean up allocated resources
	glfwTerminate();
	return 0;
}
