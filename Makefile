######################################################
## ARGUMENTS
NAME := ircserv
BOTNAME := wall-e
CXX := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++98 -g3

######################################################
## FILES
SRCS_DIR := ./srcs
BOT_DIR := ./Bot
OBJS_DIR := ./objs
BOT_OBJS_DIR := ./objs/Bot
HEADERS := header

FILES := main.cpp Client.cpp Server.cpp Channel.cpp \
    commands/invite.cpp  commands/kick.cpp commands/mode.cpp \
    commands/notice.cpp commands/part.cpp commands/ping.cpp commands/privmsg.cpp \
    commands/quit.cpp commands/registration.cpp commands/join.cpp commands/topic.cpp
BOT_FILES := bot.cpp

SRCS := $(addprefix $(SRCS_DIR)/, $(FILES))
OBJS := $(addprefix $(OBJS_DIR)/, $(FILES:.cpp=.o))

BOT_SRCS := $(addprefix $(BOT_DIR)/, $(BOT_FILES))
BOT_OBJS := $(addprefix $(BOT_OBJS_DIR)/, $(BOT_FILES:.cpp=.o))

OBJ_SUBDIRS := $(sort $(dir $(OBJS)))
BOT_OBJ_SUBDIRS := $(sort $(dir $(BOT_OBJS)))

######################################################
## RULES
all: irc bot

irc: $(NAME)

bot: $(BOTNAME)

# Règle pour créer tous les répertoires nécessaires
dirs:
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJ_SUBDIRS)
	@mkdir -p $(BOT_OBJS_DIR)

$(NAME):  $(OBJS)
	$(CXX) -I$(HEADERS) $(OBJS) -o $@

$(BOTNAME):  $(BOT_OBJS)
	$(CXX) -I$(HEADERS) $(BOT_OBJS) -o $@

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I $(HEADERS) -c $< -o $@

$(BOT_OBJS_DIR)/%.o: $(BOT_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I $(HEADERS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME) $(BOTNAME)

re: fclean all

.PHONY: all clean fclean re bot dirs