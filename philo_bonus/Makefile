SRCS = ft_split.c ft_strdup.c ft_strjoin.c philo.c helper_function.c philo_init.c \
		define_the_sem.c Simulate.c Simulate2.c Simulate3.c 
OBJS = ${SRCS:.c=.o}
NAME = philo_bonus
CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror #-fsanitize=undefined -fsanitize=thread
${NAME}: ${OBJS}
	$(CC) $(CFLAGS) ${OBJS} -o ${NAME}
 
all: ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re 
.SECONDARY: