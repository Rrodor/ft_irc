SRC = $(wildcard ./build/*.cpp)
SRC := $(filter-out ./build/client.cpp, $(SRC))

CLIENT = ./build/client.cpp

NAME = ircserv
OBJ_DIR = obj
OBJ = $(patsubst ./build/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

all: $(NAME)

client: $(OBJ)
	c++ -g -std=c++98 $(CLIENT) -o client

$(NAME): $(OBJ)
	c++ -g -std=c++98 $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: ./build/%.cpp | $(OBJ_DIR)
	c++ -g -std=c++98 -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
