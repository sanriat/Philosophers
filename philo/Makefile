NAME = philo

SRCS = ./philo.c ./philo_support.c ./philo_help.c ./philo_actions.c ./philo_checkers.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	cc -g -Wall -Wextra -Werror -pthread $(OBJS) -o $(NAME)

%.o: %.c
	cc -Wall -Wextra -Werror -g -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all