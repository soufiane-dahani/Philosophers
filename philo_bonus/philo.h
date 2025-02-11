/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:35:01 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/11 21:59:40 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <fcntl.h>
# include <limits.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_data	t_data;

typedef struct s_init
{
	int					num_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	unsigned int		must_eat_count;
}						t_init;

typedef struct s_philo
{
	int					id;
	unsigned int		meals_eaten;
	bool				has_finished_meals;
	struct timeval		last_meal_time;
	pid_t				pid;
	t_data				*data;
}						t_philo;

typedef struct s_data
{
	unsigned int		num_philos;
	bool				finished_flag;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	unsigned int		must_eat_count;
	unsigned int		meals_finished;
	struct timeval		start_time;
	int					is_dead;
	sem_t				*forks;
	sem_t				*meal_sem;
	sem_t				*print_sem;
	sem_t				*death_sem;
	sem_t				*last_meal_sem;
	sem_t				*meal_count_sem;
	t_philo				*philos;
}						t_data;

int						ft_isdigit(int c);
char					**ft_split(char const *s, char c);
size_t					ft_strlen(const char *str);
char					*ft_strjoin(char const *s1, char const *s2);
char					*join_av(int ac, char const **av);
char					*ft_strdup(const char *src);
void					*ft_memcpy(void *dest, const void *src, size_t n);
int						is_number(char *str);
int						check_all_numbers(char **res);
int						*check_number(int ac, char const **av, int *capacity);
int						*convert_to_integers(char **res, int *capacity);
long					ft_atoi(const char *str);
void					free_string_array(char **arr);
int						check_min_max(char **res);
t_data					*initialize_data(t_init init);
long long				get_current_time_ms(void);
int						start_simulation(t_data *data);
struct timeval			get_current_time(void);
void					philosopher_lifecycle(t_philo *philo);
void					cleanup_sems(t_data *data);
void					print_status(t_philo *philo, const char *status);

int						check_if_dead(t_philo *philo);
void					*monitor_death(void *arg);

#endif