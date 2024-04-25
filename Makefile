NAME = ircserv
src = $(wildcard *.c)
objs = $(src:.c=.o)
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic -g

all : $(NAME)

$(NAME): $(obj)
	c++ $(CXXFLAGS) $(objs) -o $(NAME)

clean:
	$(RM) $(objs)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re


