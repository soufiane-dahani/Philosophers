/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/02 12:41:14 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_mutexes_and_philos(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_philos)
		pthread_mutex_init(&data->forks[i], NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->death_mutex, NULL);
	i = -1;
	while (++i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = get_current_time();
		data->philos[i].data = data;
	}
}

t_data	*initialize_data(int num_philos, int time_to_die, int time_to_eat,
		int time_to_sleep)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->num_philos = num_philos;
	data->time_to_die = time_to_die;
	data->time_to_eat = time_to_eat;
	data->time_to_sleep = time_to_sleep;
	data->dead = 0;
	data->philos = malloc(sizeof(t_philo) * num_philos);
	if (!data->philos)
		return (free(data), NULL);
	data->forks = malloc(sizeof(pthread_mutex_t) * num_philos);
	if (!data->forks)
		return (free(data->philos), free(data), NULL);
	init_mutexes_and_philos(data);
	gettimeofday(&data->start_time, NULL);
	return (data);
}

void	cleanup(t_data *data)
{
	int	i;

	if (data)
	{
		if (data->forks)
		{
			i = -1;
			while (++i < data->num_philos)
				pthread_mutex_destroy(&data->forks[i]);
			free(data->forks);
		}
		if (data->philos)
			free(data->philos);
		pthread_mutex_destroy(&data->print_mutex);
		pthread_mutex_destroy(&data->death_mutex);
		free(data);
	}
}
