/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/14 18:03:09 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	pick_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep(2000);
	sem_wait(philo->data->forks);
	sem_wait(philo->data->forks);
}

void	release_forks(t_philo *philo)
{
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}
void	philosopher_lifecycle(t_philo *philo)
{
	t_data		*data;
	pthread_t	monitor_thread;

	data = philo->data;
	if (pthread_create(&monitor_thread, NULL, monitor_death, philo) != 0)
	{
		write(2, "Error: Failed to create monitor thread\n", 40);
	}
	pthread_detach(monitor_thread);
	while (1)
	{
		if (check_if_dead(philo))
			break ;
		print_status(philo, "is thinking");
		pick_forks(philo);
		print_status(philo, "has taken a fork");
		sem_wait(data->meal_sem);
		philo->last_meal_time = get_current_time();
		sem_post(data->meal_sem);
		print_status(philo, "is eating");
		usleep(data->time_to_eat * 1000);
		philo->meals_eaten++;
		if (philo->meals_eaten >= data->must_eat_count
			&& !philo->has_finished_meals)
		{
			philo->has_finished_meals = true;
			sem_wait(data->meal_sem);
			data->m->__align++;
			sem_post(data->meal_sem);
		}
		release_forks(philo);
		if (check_if_dead(philo))
			break ;
		print_status(philo, "is sleeping");
		usleep(data->time_to_sleep * 1000);
		usleep(500);
	}
	cleanup_all(philo->data);
	exit(0);
}

void	print_status(t_philo *philo, const char *status)
{
	struct timeval	current_time;
	long long		timestamp;

	sem_wait(philo->data->death_sem);
	if (philo->data->d->__align == 1)
	{
		sem_post(philo->data->death_sem);
		return ;
	}
	sem_post(philo->data->death_sem);
	gettimeofday(&current_time, NULL);
	timestamp = ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000))
		- ((philo->data->start_time.tv_sec * 1000)
			+ (philo->data->start_time.tv_usec / 1000));
	sem_wait(philo->data->print_sem);
	printf("%lld  %d %s\n", timestamp, philo->id, status);
	sem_post(philo->data->print_sem);
}

int	check_if_dead(t_philo *philo)
{
	int	is_dead;

	sem_wait(philo->data->death_sem);
	is_dead = philo->data->d->__align;
	sem_post(philo->data->death_sem);
	return (is_dead);
}

void	cleanup_sems(t_data *data)
{
	if (data->forks)
		sem_close(data->forks);
	if (data->print_sem)
		sem_close(data->print_sem);
	if (data->meal_sem)
		sem_close(data->meal_sem);
	if (data->death_sem)
		sem_close(data->death_sem);
	if (data->d)
		sem_close(data->d);
	if (data->m)
		sem_close(data->m);
	sem_unlink("/m");
	sem_unlink("/d");
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/meal_check");
	sem_unlink("/death_sem");
}
