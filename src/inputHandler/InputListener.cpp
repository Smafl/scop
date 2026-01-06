#include "InputListener.hpp"
#include "../render/Render.hpp"
#include <iostream>

namespace InputListener {

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		Render* render = static_cast<Render*>(glfwGetWindowUserPointer(window));
		if (!render) return;
	    (void)scancode;

	    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
	        glfwSetWindowShouldClose(window, GL_TRUE);
	    }

	    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
	        render->translationX -= render->delta;
	    }

	    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
	        render->translationX += render->delta;
	    }

	    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
	        render->translationY -= render->delta;
	    }

	    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
	        render->translationY += render->delta;
	    }

		// Move backward
	    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
	        render->translationZ -= render->delta;
	    }

		// Move forward
	    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
	        render->translationZ += render->delta;
	    }

		// Rotation
	    if (key == GLFW_KEY_R && action == GLFW_PRESS && ((mods & GLFW_MOD_CONTROL) || (mods & GLFW_MOD_SUPER))) {
	        render->isRotate = (render->isRotate == true) ? false : true;
	    }

		// Switch texture/color
		if (key == GLFW_KEY_T && action == GLFW_PRESS && ((mods & GLFW_MOD_CONTROL) || (mods & GLFW_MOD_SUPER))) {
			render->textureMode = !render->textureMode;
		}
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		Render* render = static_cast<Render*>(glfwGetWindowUserPointer(window));
		if (!render) return;
		(void)window;
		(void)mods;

		// MacOS trackpad -- one finger click
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			;
		}

		// MacOS trackpad -- two fingers click
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			;
		}
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		Render* render = static_cast<Render*>(glfwGetWindowUserPointer(window));
		if (!render) return;
		(void)yoffset;

		// Multiply yoffset by a sensitivity factor to control speed
    	const float scrollSensitivity = 0.1f;

		GLfloat newZ = render->translationZ + xoffset * scrollSensitivity;
		if (newZ <= render->maxZ && newZ >= render->minZ) {
			render->translationZ = newZ;
		}
	}
}
