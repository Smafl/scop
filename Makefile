
NAME = scop
CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++20 -DGL_SILENCE_DEPRECATION -MMD -MP

GREEN = \033[0;32m
RESET = \033[0m

GLAD_INCLUDE = -Iinclude
GLFW_INCLUDE = -I/opt/homebrew/opt/glfw/include
GLFW_LIB = -L/opt/homebrew/opt/glfw/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

LEEAKSAN_LDFLAG = -L../LeakSanitizer -llsan -lc++ -Wno-gnu-include-next -I ../LeakSanitize

ifdef DEBUG
	CXXFLAGS += -g -fsanitize=address
endif

SRC =	main.cpp\
		gl.cpp\
		Shader.cpp\
		ShaderProgram.cpp

OBJ_DIR = obj

OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEP = $(OBJ:.o=.d)

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(GLAD_INCLUDE) $(GLFW_INCLUDE) $(OBJ) -o $(NAME) $(GLFW_LIB)
	@printf "$(GREEN)Compiled$(RESET)\n"

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(GLAD_INCLUDE) $(GLFW_INCLUDE) -c $< -o $@

-include $(DEP)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)
	@printf "$(GREEN)Cleaned$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re
