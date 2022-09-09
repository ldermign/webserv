NAME		= webserv

INCSDIR		= incs/

SRCSDIR 	= srcs/

PARSDIR		= ${SRCSDIR}parsing/

RES_DIR		= ${SRCSDIR}response/

OBJSDIR		= objs/

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

OBJS 		= ${SRCS:%.cpp=${OBJSDIR}%.o}

DEPS		= ${SRCS:%.cpp=${OBJSDIR}%.d}

CC			= c++

CFLAGS		= -Wall  -Wextra -Werror --std=c++98 -I ${INCSDIR}

RM			= rm -rf

all:		${NAME}

${NAME}: 	${OBJS}
			${CC} ${CFLAGS} -o ${NAME} ${OBJS}

-include	${DEPS}

${OBJS}:	${OBJSDIR}%.o:%.cpp
			@mkdir -p $(dir $@)
			${CC} -I ${INCSDIR} -MMD -MP -o $@ -c $< ${CFLAGS}

clean:
			${RM} ${OBJS} ${OBJSDIR} ${DEPS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
