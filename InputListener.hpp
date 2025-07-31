#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "RenderModel.hpp"

namespace InputListener {
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	// void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	// 	(void)window;
	// 	(void)mods;

	// 	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
	// 		;
	// 	}
	// }

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
}
