######################################################
## ARGUMENTS
NAME:= ft_irc
CC:= c++
FLAGS:= -Wall -Wextra -Werror -std=c++98 -g3


######################################################
## FILES
SRCS_DIR:= ./srcs
OBJS_DIR:= ./objs

FILES:= main.cpp client.cpp

SRCS:= $(addprefix $(SRCS_DIR)/, $(FILES))
OBJS:= $(addprefix $(OBJS_DIR)/, $(FILES:.cpp=.o))

######################################################
## RULES

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $^ -o $@

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	mkdir -p $(SRCS_DIR)
	$(CC) $(FLAGS) -c $^ -o $@


clean:
	rm -rf $(NAME)

fclean: clean
	rm -rf $(OBJS_DIR)

re: fclean all

.PHONY: all clean fclean re