/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simulate_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/15 12:29:54 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

void	philosopher_eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	print_status(philo, "is eating");
	sem_wait(data->meal_sem);
	philo->last_meal_time = get_current_time();
	philo->meals_eaten++;
	sem_post(data->meal_sem);
	usleep(data->time_to_eat * 1000);
	if (philo->meals_eaten >= data->must_eat_count
		&& !philo->has_finished_meals)
	{
		philo->has_finished_meals = true;
		sem_wait(data->meal_sem);
		data->m->__align++;
		sem_post(data->meal_sem);
	}
	release_forks(philo);
}

void	philosopher_lifecycle(t_philo *philo)
{
	t_data		*data;
	pthread_t	monitor_thread;

	data = philo->data;
	if (pthread_create(&monitor_thread, NULL, monitor_death, philo) != 0)
		write(2, "Error: Failed to create monitor thread\n", 40);
	while (1)
	{
		if (check_if_dead(philo))
			break ;
		print_status(philo, "is thinking");
		pick_forks(philo);
		print_status(philo, "has taken a fork");
		philosopher_eat(philo);
		if (check_if_dead(philo))
			break ;
		print_status(philo, "is sleeping");
		usleep(data->time_to_sleep * 1000);
		usleep(500);
	}
	pthread_join(monitor_thread, NULL);
	cleanup_all(philo->data);
	exit(0);
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
