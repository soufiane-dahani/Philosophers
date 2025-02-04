/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define_the_thread.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/04 16:41:30 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	monitor_meals(t_data *data)
{
	int	i;
	int	done_eating;

	while (1)
	{
		i = 0;
		done_eating = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_lock(&data->death_mutex);
			if (data->philos[i].meals_eaten >= (int)data->must_eat_count)
				done_eating++;
			pthread_mutex_unlock(&data->death_mutex);
			i++;
		}
		if (done_eating == data->num_philos)
		{
			pthread_mutex_lock(&data->death_mutex);
			data->dead = 1;
			pthread_mutex_unlock(&data->death_mutex);
			break;
		}
		usleep(100);
	}
}


void	define_the_thread(t_data *data)
{
	int			i;
	//pthread_t	monitor_thread;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_create(&data->philos[i].thread, NULL, philosopher_routine,
				&data->philos[i]);
		i++;
	}

	// if (data->must_eat_count > 0)
	// 	pthread_create(&monitor_thread, NULL, (void *)monitor_meals, data);

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	
	// if (data->must_eat_count > 0)
	// 	pthread_join(monitor_thread, NULL);
}


long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
