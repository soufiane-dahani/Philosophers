/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define_the_thread.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/04 22:08:28 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_simulation(t_data *data)
{
	size_t		i;
	pthread_t	monitor;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL, philosopher_routine,
				&data->philos[i]) != 0)
			return (write(2, "Error: Failed to create philosopher thread\n",
					44), 1);
		i++;
	}
	if (pthread_create(&monitor, NULL, monitor_death, data) != 0)
		return (write(2, "Error: Failed to create monitor thread\n", 40), 1);
	pthread_detach(monitor);
	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	free_data(data);
	return (0);
}

void	print_status(t_philo *philo, const char *status)
{
	struct timeval	current_time;
	long long		timestamp;

	gettimeofday(&current_time, NULL);
	timestamp = ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000))
		- ((philo->data->start_time.tv_sec * 1000)
			+ (philo->data->start_time.tv_usec / 1000));
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->dead)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%lld %d %s\n", timestamp, philo->id, status);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	check_if_dead(t_philo *philo)
{
	int	dead;

	pthread_mutex_lock(&philo->data->death_mutex);
	dead = philo->data->dead;
	pthread_mutex_unlock(&philo->data->death_mutex);
	return (dead);
}

static int	check_philosopher_death(t_data *data, size_t i,
		struct timeval current_time)
{
	long long	last_meal_time_ms;
	long long	current_time_ms;

	pthread_mutex_lock(&data->philos[i].meal_mutex);
	last_meal_time_ms = (data->philos[i].last_meal_time.tv_sec * 1000)
		+ (data->philos[i].last_meal_time.tv_usec / 1000);
	current_time_ms = (current_time.tv_sec * 1000) + (current_time.tv_usec
			/ 1000);
	pthread_mutex_unlock(&data->philos[i].meal_mutex);
	if ((current_time_ms - last_meal_time_ms) >= data->time_to_die + 5)
	{
		print_status(&data->philos[i], "died");
		pthread_mutex_lock(&data->death_mutex);
		data->dead = 1;
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_death(void *arg)
{
	t_data			*data;
	size_t			i;
	struct timeval	current_time;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->num_philos)
		{
			gettimeofday(&current_time, NULL);
			if (check_philosopher_death(data, i, current_time))
				return (NULL);
			i++;
		}
		usleep(2000);
	}
	return (NULL);
}
