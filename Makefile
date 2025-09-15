######################################################
## ARGUMENTS
NAME := ircserv
CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -g3

######################################################
## FILES
SRCS_DIR := ./srcs
OBJS_DIR := ./objs
HEADERS := header

FILES := main.cpp Client.cpp Server.cpp Channel.cpp \
	commands/invite.cpp  commands/kick.cpp commands/mode.cpp \
	commands/notice.cpp commands/part.cpp commands/ping.cpp commands/privmsg.cpp \
	commands/quit.cpp commands/registration.cpp commands/join.cpp commands/topic.cpp

SRCS := $(addprefix $(SRCS_DIR)/, $(FILES))
OBJS := $(addprefix $(OBJS_DIR)/, $(FILES:.cpp=.o))
OBJ_SUBDIRS := $(sort $(dir $(OBJS)))

######################################################
## RULES
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX)  -I$(HEADERS) $(OBJS) -o $@

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJ_SUBDIRS)
	$(CXX) $(CXXFLAGS) -I $(HEADERS) -c $< -o $@

# Créer les répertoires d'objets
$(OBJS_DIR) $(OBJ_SUBDIRS):
	mkdir -p $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re