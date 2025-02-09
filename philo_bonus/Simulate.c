/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/09 22:37:25 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philosopher_lifecycle(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	while (1)
	{
		sem_wait(data->print_sem);
		printf("Philosopher %d is thinking\n", philo->id);
		sem_post(data->print_sem);
		if (philo->id % 2 == 0)
			usleep(500);
		sem_wait(data->forks);
		sem_wait(data->forks);
		sem_wait(data->print_sem);
		printf("Philosopher %d is eating\n", philo->id);
		sem_post(data->print_sem);
		usleep(data->time_to_eat * 1000);
		sem_wait(data->meal_sem);
		philo->last_meal_time = get_current_time();
		sem_post(data->meal_sem);
		sem_post(data->forks);
		sem_post(data->forks);
		sem_wait(data->print_sem);
		printf("Philosopher %d is sleeping\n", philo->id);
		sem_post(data->print_sem);
		usleep(data->time_to_sleep * 1000);
	}
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
