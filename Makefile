NAME 		= TCPchat
CC			= g++
CFLAGS		= -fsanitize=address -Wall -Wextra  -lpthread -std=c++20

RM 			= rm -f

SRC_DIR		= ./src/
INCLUDE_DIR	= ./include/
OBJ_DIR		= ./build/

SRCSC 		=  src/ServerTCP.cpp \
				src/ClientTCP.cpp \
				src/Logger.cpp \
				src/TCPchat.cpp

OBJS		= ${SRCSC:%.cpp=${OBJ_DIR}/%.o}

all: ${NAME}

${OBJ_DIR}/%.o: %.cpp
			@mkdir -p ${@D}
			@printf "\033[2K\033[0;35m${NAME}\033[0;0m:    \033[0;33mCompilation...    \033[0;31m%-15.15s\033[0;0m\r" $(notdir ${<})

			@${CC} ${CFLAGS} -c $< -o $@

${NAME}:	header ${OBJS}
			@printf "\033[2K\033[0;35m${NAME}\033[0;0m:    \033[0;32mCompleted         \033[0;31m----\033[0;0m          \r"
			@printf "\n\033[0;0m⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤\n\033[0;35m${NAME}\033[0;0m:    \033[0;32mReady             \033[0;31m----          \033[0;0m\n⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤\n"
			
			@${CC} ${CFLAGS} ${OBJS} -o $@

header:
			@printf "\033[0;0m⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤\n\033[0;35mNAME          \033[0;0mSTATUS            \033[0;31mFILE          \033[0;0m\n⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤⏤\n"

clean:
			@${RM} ${OBJS}
			@printf "\033[0;35mPROJECT\033[0;0m:      \033[0;32mCleaned\033[0;0m\n"

fclean:		clean
			@${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
