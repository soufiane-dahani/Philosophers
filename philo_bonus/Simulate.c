/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/11 22:39:35 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philosopher_lifecycle(t_philo *philo)
{
	t_data		*data;
	pthread_t	monitor_thread;

	data = philo->data;
	if (pthread_create(&monitor_thread, NULL, monitor_death, philo) != 0)
	{
		write(2, "Error: Failed to create monitor thread\n", 40);
		return ;
	}
	while (!check_if_dead(philo))
	{
		if (data->death_sem->__align != 1)
			return ;
		print_status(philo, "is thinking");
		if (philo->id % 2 == 0)
			usleep(1000);
		sem_wait(data->forks);
		sem_wait(data->forks);
		print_status(philo, "has taken a fork");
		sem_wait(data->last_meal_sem);
		philo->last_meal_time = get_current_time();
		sem_post(data->last_meal_sem);
		print_status(philo, "is eating");
		usleep(data->time_to_eat * 1000);
		sem_wait(data->meal_sem);
		philo->meals_eaten++;
		sem_wait(data->meal_count_sem);
		if (philo->meals_eaten >= data->must_eat_count
			&& !philo->has_finished_meals)
		{
			philo->has_finished_meals = true;
			data->meals_finished++;
			philo->data->meal_count_sem->__align++;
		}
		sem_post(data->meal_count_sem);
		sem_post(data->meal_sem);
		sem_post(data->forks);
		sem_post(data->forks);
		print_status(philo, "is sleeping");
		usleep(data->time_to_sleep * 1000);
	}
	pthread_join(monitor_thread, NULL);
}

void	print_status(t_philo *philo, const char *status)
{
	struct timeval	current_time;
	long long		timestamp;

	if (check_if_dead(philo))
		return ;
	if (philo->data->death_sem->__align != 1)
		return ;
	gettimeofday(&current_time, NULL);
	timestamp = ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000))
		- ((philo->data->start_time.tv_sec * 1000)
			+ (philo->data->start_time.tv_usec / 1000));
	sem_wait(philo->data->death_sem);
	sem_wait(philo->data->print_sem);
	printf("%lld %d %s\n", timestamp, philo->id, status);
	sem_post(philo->data->print_sem);
	sem_post(philo->data->death_sem);
}

int	check_if_dead(t_philo *philo)
{
	int	is_dead;

	sem_wait(philo->data->death_sem);
	is_dead = philo->data->is_dead;
	sem_post(philo->data->death_sem);
	return (is_dead);
}

void	cleanup_sems(t_data *data)
{
	sem_close(data->forks);
	sem_close(data->print_sem);
	sem_close(data->meal_sem);
	sem_close(data->death_sem);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/meal_check");
	sem_unlink("/death_sem");
}
