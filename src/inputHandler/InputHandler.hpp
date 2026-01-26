#pragma once

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

struct InputData;

class InputHandler {
public:
    static InputHandler &getInstance();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_position_callback(GLFWwindow *window, double xPos, double yPos);

private:
    void handleKeyInput(InputData* inputData, int key, int scancode, int action, int mods);
    void handleMouseButton(InputData* inputData, int button, int action, int mods);
    void handleScroll(InputData* inputData, double xoffset, double yoffset);
    void handleMouseMove(InputData* inputData, double xPos, double yPos);

	InputHandler();
    InputHandler(const InputHandler &other);
    InputHandler& operator=(const InputHandler &API_OBSOLETED_WITH_REPLACEMENT);
};
