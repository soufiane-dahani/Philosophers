SRCS = ft_split_bonus.c ft_strdup_bonus.c ft_strjoin_bonus.c philo_bonus.c helper_function_bonus.c philo_init_bonus.c \
		define_the_sem_bonus.c Simulate_bonus.c Simulate2_bonus.c Simulate3_bonus.c 
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