
##### Compilation ##############################################################

CC			= c++
CFLAGS		= -Wall -Werror -Wextra -g -std=c++98
RM			= rm -rf

# SRCS:
NAME		=   irc
FILES		=	main.cpp \
				Client.cpp \
				Server.cpp \
				Channel.cpp \

# OBJS
SRCS		=	$(addprefix , $(FILES))	\

OBJS		= $(SRCS:.cpp=.o)

##### Rules ####################################################################

all: $(NAME)

.cpp.o: $(SRCS)
	$(CC) $(CFLAGS) -c -o $@ $< 

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clear:
	clear

clean:
	$(RM) $(OBJS)

fclean: clear clean
	$(RM) $(NAME)

re: fclean all 

.PHONY: all clean clear fclean re