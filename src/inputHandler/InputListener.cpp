#include "InputListener.hpp"
#include "../window/Window.hpp"
#include <iostream>

namespace InputListener {

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		InputData* inputData = static_cast<InputData*>(glfwGetWindowUserPointer(window));
		if (!inputData) return;
	    (void)scancode;

		// Close by ESC
	    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
	        glfwSetWindowShouldClose(window, GL_TRUE);
	    }

		// Translation
	    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
	        inputData->transformation->transform.translationX -= inputData->transformation->animationState.delta;
	    }

	    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
	        inputData->transformation->transform.translationX += inputData->transformation->animationState.delta;
	    }

	    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
	        inputData->transformation->transform.translationY -= inputData->transformation->animationState.delta;
	    }

	    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
	        inputData->transformation->transform.translationY += inputData->transformation->animationState.delta;
	    }

		// Move backward
	    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
	        inputData->transformation->transform.translationZ -= inputData->transformation->animationState.delta;
	    }

		// Move forward
	    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
	        inputData->transformation->transform.translationZ += inputData->transformation->animationState.delta;
	    }

		// Rotation
	    if (key == GLFW_KEY_R && action == GLFW_PRESS && ((mods & GLFW_MOD_CONTROL) || (mods & GLFW_MOD_SUPER))) {
	        inputData->transformation->animationState.isRotate = (inputData->transformation->animationState.isRotate == true) ? false : true;
	    }

		// Switch texture/color
		if (key == GLFW_KEY_T && action == GLFW_PRESS && ((mods & GLFW_MOD_CONTROL) || (mods & GLFW_MOD_SUPER))) {
			inputData->material->materialBlend.textureMode = !inputData->material->materialBlend.textureMode;
			inputData->material->materialBlend.targetMixValue = inputData->material->materialBlend.textureMode ? 1.0f : 0.0f;
		}

		// Show face colors
		if (key == GLFW_KEY_F && action == GLFW_PRESS && ((mods & GLFW_MOD_CONTROL) || (mods & GLFW_MOD_SUPER))) {
			inputData->material->materialBlend.faceColorMode = !inputData->material->materialBlend.faceColorMode;
    		inputData->material->materialBlend.textureMode = false;
    		inputData->material->materialBlend.targetMixValue = 0.0f;
		}
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
		InputData* inputData= static_cast<InputData*>(glfwGetWindowUserPointer(window));
		if (!inputData) return;
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
		InputData* inputData = static_cast<InputData*>(glfwGetWindowUserPointer(window));
		if (!inputData) return;
		(void)yoffset;

		// Multiply yoffset by a sensitivity factor to control speed
    	const float scrollSensitivity = 0.1f;

		GLfloat newZ = inputData->transformation->transform.translationZ + xoffset * scrollSensitivity;
		if (newZ <= inputData->transformation->maxZ && newZ >= inputData->transformation->minZ) {
			inputData->transformation->transform.translationZ = newZ;
		}
	}
}
