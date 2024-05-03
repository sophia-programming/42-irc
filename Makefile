NAME = ircserv
srcs = srcs/main.cpp
srcs += srcs/Server/createSocket.cpp
srcs += srcs/Server/handleConnections.cpp

includes = -I./includes
objs = $(srcs:.cpp=.o)

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -g $(includes)

all : $(NAME)

$(NAME): $(objs)
	c++ $(CXXFLAGS) $(includes) $(objs) -o $(NAME)

clean:
	$(RM) $(objs)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re


