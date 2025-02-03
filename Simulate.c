/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/03 12:27:54 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_sleep(t_philo *philo)
{
	long	start_time;

	start_time = get_current_time();
	if (!philo->data->dead)
		printf("philo %d is sleeping\n", philo->id);
	while ((get_current_time() - start_time < philo->data->time_to_sleep
		&& !philo->data->dead))
	{
		usleep(100);
	}
}
void	think(t_philo *philo)
{
	if (!philo->data->dead)
	{
		printf("philo %d is thinking\n", philo->id);
		usleep(1000);
	}
}


void	take_forks(t_philo *philo)
{
	int	first_fork;
	int	second_fork;

	if (philo->id - 1 < philo->id % philo->data->num_philos)
	{
		first_fork = philo->id - 1;
		second_fork = philo->id % philo->data->num_philos;
	}
	else
	{
		first_fork = philo->id % philo->data->num_philos;
		second_fork = philo->id - 1;
	}
	if(!philo->data->dead)
	{
	printf("Philo %d is trying to take fork %d\n", philo->id, first_fork);
	pthread_mutex_lock(&philo->data->forks[first_fork]);
	printf("Philo %d is trying to take fork %d\n", philo->id, second_fork);
	pthread_mutex_lock(&philo->data->forks[second_fork]);
	}
}

void	eat(t_philo *philo)
{
	long	start_time;

	if (!philo->data->dead)
		printf("Philo %d is eating\n", philo->id);
	philo->last_meal_time = get_current_time();
	start_time = get_current_time();
	while ((get_current_time() - start_time < philo->data->time_to_eat)
		&& !philo->data->dead)
	{
		usleep(100);
	}
	if (!philo->data->dead)
	{
		printf("Philo %d has finished eating\n", philo->id);
		philo->meals_eaten++;
	}
}

void	release_forks(t_philo *philo)
{
	int	left_fork;
	int	right_fork;

	left_fork = philo->id - 1;
	right_fork = philo->id % philo->data->num_philos;
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(&philo->data->forks[right_fork]);
		pthread_mutex_unlock(&philo->data->forks[left_fork]);
	}
	else
	{
		pthread_mutex_unlock(&philo->data->forks[left_fork]);
		pthread_mutex_unlock(&philo->data->forks[right_fork]);
	}
}
