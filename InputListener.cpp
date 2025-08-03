#include "InputListener.hpp"
#include "RenderModel.hpp"
#include <iostream>

namespace InputListener {

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		RenderModel* model = static_cast<RenderModel*>(glfwGetWindowUserPointer(window));
		if (!model) return;
	    (void)scancode;

	    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
	        glfwSetWindowShouldClose(window, GL_TRUE);
	    }

	    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
	        model->translationX -= model->delta;
	    }

	    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
	        model->translationX += model->delta;
	    }

	    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
	        model->translationY -= model->delta;
	    }

	    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
	        model->translationY += model->delta;
	    }

		// Move backward
	    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
	        model->translationZ -= model->delta;
	    }

		// Move forward
	    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
	        model->translationZ += model->delta;
	    }

	    if (key == GLFW_KEY_R && action == GLFW_PRESS && ((mods & GLFW_MOD_CONTROL) || (mods & GLFW_MOD_SUPER))) {
	        model->isRotate = (model->isRotate == true) ? false : true;
	    }
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		RenderModel* model = static_cast<RenderModel*>(glfwGetWindowUserPointer(window));
		if (!model) return;
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
		RenderModel* model = static_cast<RenderModel*>(glfwGetWindowUserPointer(window));
		if (!model) return;
		(void)yoffset;

		// Multiply yoffset by a sensitivity factor to control speed
    	const float scrollSensitivity = 0.1f;

		GLfloat newZ = model->translationZ + xoffset * scrollSensitivity;
		if (newZ <= model->maxZ && newZ >= model->minZ) {
			model->translationZ = newZ;
		}
	}
}
