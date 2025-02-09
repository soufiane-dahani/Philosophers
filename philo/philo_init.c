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

static int	init_forks(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			write(2, "Error: Mutex initialization failed\n", 35);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	init_global_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (write(2, "Error: Mutex initialization failed\n", 35), 1);
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
		return (write(2, "Error: Mutex initialization failed\n", 35), 1);
	if (pthread_mutex_init(&data->meal_mutex, NULL) != 0)
		return (write(2, "Error: Mutex initialization failed\n", 35), 1);
	return (0);
}

static void	init_philosophers(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = get_current_time();
		data->philos[i].data = data;
		pthread_mutex_init(&data->philos[i].meal_mutex, NULL);
		i++;
	}
}

static int	init_mutexes_and_philos(t_data *data)
{
	if (init_forks(data))
		return (1);
	if (init_global_mutexes(data))
		return (1);
	init_philosophers(data);
	return (0);
}

t_data	*initialize_data(t_init init)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->num_philos = init.num_philos;
	data->time_to_die = init.time_to_die;
	data->time_to_eat = init.time_to_eat;
	data->time_to_sleep = init.time_to_sleep;
	data->must_eat_count = init.must_eat_count;
	data->dead = 0;
	data->philos = malloc(sizeof(t_philo) * init.num_philos);
	data->forks = malloc(sizeof(pthread_mutex_t) * init.num_philos);
	if (!data->philos || !data->forks)
		return (free(data->philos), free(data->forks), free(data), NULL);
	if (init_mutexes_and_philos(data))
		return (free(data->forks), free(data->philos), free(data), NULL);
	gettimeofday(&data->start_time, NULL);
	return (data);
}
