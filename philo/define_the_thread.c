/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define_the_thread.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/13 18:39:03 by sodahani         ###   ########.fr       */
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
	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
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
	if (check_if_dead(philo))
		return ;
	if (!check_if_dead(philo))
	{
		pthread_mutex_lock(&philo->data->print_mutex);
		printf("%lld %d %s\n", timestamp, philo->id, status);
		pthread_mutex_unlock(&philo->data->print_mutex);
	}
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
	long long	timestamp;

	pthread_mutex_lock(&data->philos[i].meal_mutex);
	last_meal_time_ms = (data->philos[i].last_meal_time.tv_sec * 1000)
		+ (data->philos[i].last_meal_time.tv_usec / 1000);
	current_time_ms = (current_time.tv_sec * 1000) + (current_time.tv_usec
			/ 1000);
	pthread_mutex_unlock(&data->philos[i].meal_mutex);
	if ((current_time_ms - last_meal_time_ms) >= data->time_to_die)
	{
		pthread_mutex_lock(&data->death_mutex);
		data->dead = 1;
		gettimeofday(&current_time, NULL);
		timestamp = ((current_time.tv_sec * 1000) + (current_time.tv_usec
					/ 1000)) - ((data->start_time.tv_sec * 1000)
				+ (data->start_time.tv_usec / 1000));
		pthread_mutex_lock(&data->print_mutex);
		printf("%lld %d %s\n", timestamp, data->philos[i].id, "died");
		pthread_mutex_unlock(&data->print_mutex);
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
		if (check_meals(data))
			return (NULL);
		while (i < data->num_philos)
		{
			gettimeofday(&current_time, NULL);
			if (check_philosopher_death(data, i, current_time))
				return (NULL);
			i++;
		}
		if (data->num_philos % 2 != 0)
			usleep(200);
	}
	return (NULL);
}
