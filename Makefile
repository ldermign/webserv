NAME		= webserv

SRCS		=	${SRCSDIR}webserv.cpp \

#server
SRCS		+=	${SRCSDIR}set_server/Socket.cpp \
				${SRCSDIR}set_server/FtServer.cpp \

# parsing 
SRCS		+=	${PARSDIR}ConfigurationFile.cpp \
				${PARSDIR}parsing_config_file.cpp \
				${PARSDIR}utils_parsing.cpp

INCSDIR		= incs/

SRCSDIR 	= srcs/

PARSDIR		= srcs/parsing/

OBJSDIR		= objs

OBJS 		= ${SRCS:%.cpp=${OBJSDIR}/%.o}

DEPS		= ${SRCS:.cpp=.d}

CC			= c++

CFLAGS		= -Wall  -Wextra -Werror --std=c++98 -I ${INCSDIR} -g3 

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
