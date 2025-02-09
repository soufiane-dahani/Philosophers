SRCS = ft_split.c ft_strdup.c ft_strjoin.c philo.c helper_function.c philo_init.c \
		define_the_thread.c Simulate.c meals.c
SRCSB =

OBJS = ${SRCS:.c=.o}
OBJSB = ${SRCSB:.c=.o}
NAME = philo
NAMEB = philo_bonus
CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror #-fsanitize=undefined -fsanitize=thread
${NAME}: ${OBJS}
	$(CC) $(CFLAGS) ${OBJS} -o ${NAME}

bonus: ${NAMEB}

${NAMEB}: ${OBJSB}
	$(CC) $(CFLAGS) ${OBJSB} -o ${NAMEB}
 
all: ${NAME}

clean:
	${RM} ${OBJS} ${OBJSB}

fclean: clean
	${RM} ${NAME} ${NAMEB} 

re: fclean all

.PHONY: all clean fclean re bonus
.SECONDARY: