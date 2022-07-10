NAME		= webserv

SRCS		=	${SRCSDIR}webserv.cpp \
			 	${SRCSDIR}loop_server.cpp \
			 	${SRCSDIR}request.cpp \
# parsing 
SRCS		+=	${PARSDIR}ConfigurationFile.cpp

INCSDIR		= incs/

SRCSDIR 	= srcs/

PARSDIR		= srcs/parsing/

OBJSDIR		= objs

OBJS 		= ${SRCS:%.cpp=${OBJSDIR}/%.o}

DEPS		= ${SRCS:.cpp=.d}

CC			= c++

CFLAGS		= -Wall -Wextra -Werror --std=c++11 -I ${INCSDIR} -g3

RM			= rm -rf

all:		${NAME}

${NAME}: 	${OBJS}
			${CC} ${CFLAGS} -o ${NAME} ${OBJS}

-include	${DEPS}

${OBJS}:	${OBJSDIR}/%.o:%.cpp
			mkdir -p ${@D}
			${CC} -I ${INCSDIR} -MMD -MP -o $@ -c $< ${CFLAGS}

clean:
			${RM} ${OBJS} ${OBJSDIR} ${DEPS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
