SRCS = 
SRCSB =

OBJS = ${SRCS:.c=.o}
OBJSB = ${SRCSB:.c=.o}
NAME = philo
NAMEB = philo_bonus
CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror

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