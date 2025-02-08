/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/04 17:31:55 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_mutexes_and_philos(t_data *data)
{
	size_t	i;

	// Initialize mutexes for forks
	for (i = 0; i < data->num_philos; i++)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			write(2, "Error: Mutex initialization failed\n", 35);
			return (1);
		}
	}
	// Initialize global mutexes
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0 ||
		pthread_mutex_init(&data->death_mutex, NULL) != 0 ||
		pthread_mutex_init(&data->meal_mutex, NULL) != 0)
	{
		write(2, "Error: Mutex initialization failed\n", 35);
		return (1);
	}
	// Initialize philosophers
	for (i = 0; i < data->num_philos; i++)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = get_current_time();
		data->philos[i].data = data;
		pthread_mutex_init(&data->philos[i].meal_mutex, NULL);
	}
	return (0);
}

t_data	*initialize_data(int num_philos, int time_to_die, int time_to_eat,
		int time_to_sleep, unsigned int must_eat_count)
{
	t_data	*data;

	// Allocate memory for the main data structure
	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	// Initialize variables
	data->num_philos = num_philos;
	data->time_to_die = time_to_die;
	data->time_to_eat = time_to_eat;
	data->time_to_sleep = time_to_sleep;
	data->must_eat_count = must_eat_count;
	data->dead = 0;
	// Allocate memory for philosophers
	data->philos = malloc(sizeof(t_philo) * num_philos);
	if (!data->philos)
		return (free(data), NULL);
	// Allocate memory for forks (mutex array)
	data->forks = malloc(sizeof(pthread_mutex_t) * num_philos);
	if (!data->forks)
		return (free(data->philos), free(data), NULL);
	// Initialize mutexes & philosophers
	if (init_mutexes_and_philos(data))
	{
		free(data->forks);
		free(data->philos);
		free(data);
		return (NULL);
	}
	// Record simulation start time
	gettimeofday(&data->start_time, NULL);
	return (data);
}

long long	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000LL) + ((long long)tv.tv_usec / 1000LL);
}

struct timeval	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv);
}
