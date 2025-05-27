
NAME = scop
CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -std=c++20
CXXFLAGS += -DGL_SILENCE_DEPRECATION

GREEN = \033[0;32m
RESET = \033[0m

GLAD_INCLUDE = -Iinclude
GLFW_INCLUDE = -I/opt/homebrew/opt/glfw/include
GLFW_LIB = -L/opt/homebrew/opt/glfw/lib -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

LEEAKSAN_LDFLAG = -L../LeakSanitizer -llsan -lc++ -Wno-gnu-include-next -I ../LeakSanitize

ifndef LEN
	CXXFLAGS += -Wall -Werror -Wextra -std=c++20
endif

ifdef DEBUG
	CXXFLAGS += -g -fsanitize=address
endif

SRC =	main.cpp\
		gl.cpp

OBJ_DIR = obj
# VISUALS_DIR = /visuals
# PARSE_DIR = /parser

OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
# HEADERS =

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(NAME): $(OBJ)
	@$(CXX) $(CXXFLAGS) $(GLAD_INCLUDE) $(GLFW_INCLUDE) $(OBJ) -o $(NAME) $(GLFW_LIB)
	@printf "$(GREEN)Compiled$(RESET)\n"

$(OBJ_DIR)/%.o: %.cpp Makefile | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(GLAD_INCLUDE) $(GLFW_INCLUDE) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)
	@printf "$(GREEN)Cleaned$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re
