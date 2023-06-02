#update on every project
NAME := ircserv
LST_SRC :=	main \
			Core/Server \
			Core/Client \
			Core/Channel \
			Core/RPL \
			Core/Parsing \
			Commands/INVITE \
			Commands/JOIN \
			Commands/KICK \
			Commands/MODE \
			Commands/NICK \
			Commands/NOTICE \
			Commands/PASS \
			Commands/PRIVMSG \
			Commands/QUIT \
			Commands/TOPIC \
			Commands/USER

BONUS_SRC := bot/Bot.cpp \
				bot/main.cpp

BONUS_NAME := Mildred

#update to match c or cpp
CC := c++
FILE_EXT := .cpp
HEADER_EXT := .hpp
CPPFLAGS := -std=c++98

#update if needed
CFLAGS = -Wall -Wextra -Werror -MD -I$(DIR_INC)# -fsanitize=address -g3
DIR_SRC := sources#.
SUB_DIR_LST := Core Commands

#shouldn't need to update
RM := rm -rf
MD := mkdir -p

DIR_INC := includes
DIR_OBJ := .object

OBJ=$(addprefix $(DIR_OBJ)/,$(addsuffix .o,$(LST_SRC)))
DEP=$(addprefix $(DIR_OBJ)/,$(addsuffix .d,$(LST_SRC)))
SUB_DIR=$(addprefix $(DIR_OBJ)/,$(SUB_DIR_LST))

all : $(NAME)

bonus : $(NAME)
		$(CC) $(CFLAGS) $(CPPFLAGS) $(BONUS_SRC) -o $(BONUS_NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

$(DIR_OBJ)/%.o : $(DIR_SRC)/%$(FILE_EXT) Makefile | $(DIR_OBJ) $(SUB_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

$(DIR_OBJ)	:
		$(MD) $(DIR_OBJ);

$(SUB_DIR)	:
	$(MD) $@

clean :
	$(RM) $(DIR_OBJ) Mildred.d

fclean : clean
	$(RM) $(NAME) ${BONUS_NAME}

re : fclean all

.PHONY : all clean fclean re

-include $(DEP)
