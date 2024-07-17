NAME	= ircserv
CC		= c++
CFLAGS	= -Wall -Wextra -Werror -std=c++98
SRCS	= main.cpp RPN.cpp
OBJS    = $(SRCS:.cpp=.o)
GREEN	= \033[0;32m
RED		= \033[0;31m
MAGENTA = \033[0;35m
CYAN	= \033[0;36m
CODE 	= \033[m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@
	@echo "$(GREEN)✅ $(NAME) created!$(CODE)"

%.o: %.cpp
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(MAGENTA)⌛ Compiling $<$(CODE)"

clean:
	@rm -rf $(OBJS)
	@echo "$(RED)🗑️  Removed object files$(CODE)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(RED)🗑️  Removed $(NAME)$(CODE)"
	
re: fclean all

.PHONY: all clean fclean re