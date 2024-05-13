NAME = ircserv
srcs = srcs/main.cpp \
srcs/client/Client.cpp \
srcs/server/Server.cpp \
srcs/server/Signal.cpp \
srcs/server/User.cpp \
srcs/server/Message.cpp

includes = -I./includes
objs = $(srcs:.cpp=.o)

#CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -g $(includes)
CXXFLAGS = -std=c++98 -g $(includes)

all : $(NAME)

$(NAME): $(objs)
	c++ $(CXXFLAGS) $(includes) $(objs) -o $(NAME)

clean:
	$(RM) $(objs)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re


