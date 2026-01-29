
NAME = scop
CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -MMD -MP

UNAME_S := $(shell uname -s)

GLAD_INCLUDE = -Iinclude

PKG_CONFIG      := pkg-config
PKG_CONFIG_PATH := $(HOME)/.local/lib/pkgconfig
export PKG_CONFIG_PATH

ifeq ($(UNAME_S),Darwin)
	CXXFLAGS += -std=c++20
	CXXFLAGS += -DGL_SILENCE_DEPRECATION

	GLFW_INCLUDE = -I/opt/homebrew/opt/glfw/include
	GLM_INCLUDE = -I/opt/homebrew/include/glm
	GLFW_LIB = -L/opt/homebrew/opt/glfw/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else
	CXXFLAGS += -std=c++2a

	GLFW_INCLUDE = $(shell $(PKG_CONFIG) --cflags glfw3)
	GLM_INCLUDE = -I$(HOME)/.local/include
	GLFW_LIB = $(shell $(PKG_CONFIG) --libs glfw3) -lGL -lX11 -lXi -lXrandr -lXinerama -lXcursor -ldl
endif

LEEAKSAN_LDFLAG = -L../LeakSanitizer -llsan -lc++ -Wno-gnu-include-next -I ../LeakSanitize

GREEN = \033[0;32m
RESET = \033[0m

ifeq ($(DEBUG), 1)
	CXXFLAGS += -g -fsanitize=address
endif

SRC =	src/main.cpp\
		glad.cpp\
		src/modelLoader/RenderModelLoader.cpp\
		src/window/Window.cpp\
		src/inputHandler/InputHandler.cpp\
		src/shaders/Shader.cpp\
		src/shaders/ShaderProgram.cpp\
		src/render/Render.cpp\
		src/render/RenderDraw.cpp\
		src/texture/Texture.cpp\
		src/texture/BMPLoader.cpp\
		src/matrixMath/MatrixTransform.cpp\
		src/scene/Transformation.cpp\
		src/scene/Camera.cpp

OBJ_DIR = obj

OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEP = $(OBJ:.o=.d)

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(GLAD_INCLUDE) $(GLFW_INCLUDE) $(GLM_INCLUDE) $(OBJ) -o $(NAME) $(GLFW_LIB)
	@printf "$(GREEN)Compiled$(RESET)\n"

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(GLAD_INCLUDE) $(GLFW_INCLUDE) $(GLM_INCLUDE) -c $< -o $@

-include $(DEP)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)
	@printf "$(GREEN)Cleaned$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re
