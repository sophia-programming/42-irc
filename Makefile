NAME = ircserv
SRC_DIR	=	./srcs
SRCS	=	$(shell find $(SRC_DIR) -type f -name '*.cpp')

INCLUDES	=	-I./includes
OBJ_DIR	=	./objs
OBJS	=	$(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

#CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -g $(includes)
CXXFLAGS = -std=c++98 -g $(INCLUDES) -fsanitize=address -g

all : $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $^ -o $@

clean:
	$(RM) -rf $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
