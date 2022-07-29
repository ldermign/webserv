NAME		= webserv

SRCS		=	${SRCSDIR}webserv.cpp \

#server
SRCS		+=	${SRCSDIR}set_server/Socket.cpp \
				${SRCSDIR}set_server/FtServer.cpp 

# Parse 
SRCS		+=	${PARSDIR}Parse.cpp \
				${PARSDIR}Location.cpp \
				${PARSDIR}Server.cpp \
				${PARSDIR}Config.cpp

# Response creation
SRCS		+=  ${RES_DIR}Response.cpp \
				${RES_DIR}ResponseHeader.cpp \
				${RES_DIR}ResponseBody.cpp

INCSDIR		= incs/

SRCSDIR 	= srcs/

PARSDIR		= srcs/parsing/

RES_DIR		= srcs/response/

OBJSDIR		= objs

OBJS 		= ${SRCS:%.cpp=${OBJSDIR}/%.o}

DEPS		= ${OBJS:.o=.d}

CC			= c++

CFLAGS		= -Wall  -Wextra -Werror --std=c++98 -I ${INCSDIR} -g3 -fsanitize=address

RM			= rm -rf

all:		${NAME}

${NAME}: 	${OBJS} ${DEPS}
			${CC} ${CFLAGS} -o ${NAME} ${OBJS}

#-include	${DEPS}

${OBJS}:	${OBJSDIR}/%.o:%.cpp
			mkdir -p ${@D}
			${CC} -I ${INCSDIR} -MMD -MP -o $@ -c $< ${CFLAGS}

clean:
			${RM} ${OBJS} ${OBJSDIR} ${DEPS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
