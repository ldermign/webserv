NAME		= webserv

SRCS		= ${SRCSDIR}webserv.cpp

INCSDIR		= ./incs/

SRCSDIR 	= ./srcs/

OBJS 		= $(SRCS:.cpp=.o)

DEPS		= ${SRCS:.cpp=.d}

CC			= c++

CFLAGS		= -Wall -Wextra -Werror --std=c++98 #-g3

RM			= rm -rf

all:		${NAME}

${NAME}: 	${OBJS}
			${CC} ${CFLAGS} -o ${NAME} ${OBJS}

-include	${DEPS}

%.o:%.cpp
			${CC} -I ${INCSDIR} -MMD -MP -o $@ -c $< ${CFLAGS}

clean:
			${RM} ${OBJS} ${DEPS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
