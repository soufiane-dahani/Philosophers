/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:35:01 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/04 17:34:04 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	int					id;
	unsigned int		meals_eaten;
	bool				has_fork;
	struct timeval		last_meal_time;
	pthread_t			thread;
	pthread_t			monitor_thread;
	pthread_t			monitor_dead;
	pthread_mutex_t meal_mutex; // Added for thread safety
	t_data				*data;
}						t_philo;

typedef struct s_data
{
	unsigned int		num_philos;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	unsigned int		must_eat_count;
	bool				dead;
	struct timeval		start_time;
	pthread_mutex_t		*forks;
	pthread_mutex_t		meal_mutex;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		death_mutex;
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
t_data					*initialize_data(int num_philos, int time_to_die,
							int time_to_eat, int time_to_sleep,
							unsigned int must_eat_count);

long long				get_current_time_ms(void);
int						start_simulation(t_data *data);
void					pick_forks(t_philo *philo);
void					release_forks(t_philo *philo);

void					*monitor_death(void *arg);
void					print_status(t_philo *philo, const char *status);

struct timeval			get_current_time(void);

void					*philosopher_routine(void *arg);

#endif