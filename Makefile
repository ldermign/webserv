SRC = \
		webserv.cpp \

SRCSDIR = ./srcs

SRCS = ${SRC:%.cpp=$(SRCSDIR)/%.cpp}

OBJS = $(SRCS:.cpp=.o)

NAME = webserv

CC = c++

CFLAGS = -Wall -Wextra -Werror --std=c++98

RM = rm -rf

.c.o :
	$(CC)  $(CFLAGS) -c $< -o ${<:.cpp=.o}

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS)

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
