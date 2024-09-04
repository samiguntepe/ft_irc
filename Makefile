NAME	= ircserv
CC		= c++
CFLAGS	= -Wall -Wextra -Werror -std=c++98
SRC_DIR = srcs
SRCS	= ./srcs/Channel.cpp \
		./srcs/main.cpp \
		./srcs/Server.cpp \
		./srcs/Utils.cpp \
		./srcs/Utils_Server.cpp \
		./srcs/commands/Bot.cpp \
		./srcs/commands/Cap.cpp \
		./srcs/commands/Invite.cpp \
		./srcs/commands/Join.cpp \
		./srcs/commands/Kick.cpp \
		./srcs/commands/Mode.cpp \
		./srcs/commands/Nick.cpp \
		./srcs/commands/Notice.cpp \
		./srcs/commands/Part.cpp \
		./srcs/commands/Pass.cpp \
		./srcs/commands/Privmsg.cpp \
		./srcs/commands/Quit.cpp \
		./srcs/commands/Topic.cpp \
		./srcs/commands/User.cpp \
		./srcs/commands/Who.cpp


OBJS    = $(SRCS:.cpp=.o)
GREEN	= \033[0;32m
RED		= \033[0;31m
MAGENTA = \033[0;35m
CYAN	= \033[0;36m
CODE 	= \033[m
CLEAR_LINE = \033[2K

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@
	@printf "\n$(GREEN)✅ $(NAME) created!$(CODE)\n"

%.o: %.cpp
	@printf "$(CLEAR_LINE)$(MAGENTA)⌛ Compiling $<\r$(CODE)"
	@$(CC) $(CFLAGS) -c $< -o $@

bonus:
	@$(CC) $(CFLAGS) $(SRC_DIR)/bot/*.cpp $(SRC_DIR)/Utils.cpp $^ -o bot
	@echo "\n\033[32m   Bot Ready... \033[0m\n  "

clean:
	@rm -rf $(OBJS)
	@echo "$(RED)🗑️  Removed object files$(CODE)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(RED)🗑️  Removed $(NAME)$(CODE)"
	@rm -rf bot
	
re: fclean all

.PHONY: all clean fclean re