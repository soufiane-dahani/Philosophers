/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/09 23:41:19 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philosopher_lifecycle(t_philo *philo)
{
	t_data			*data;
	struct timeval	current_time;
	long long		last_meal_time_ms;
	long long		current_time_ms;

	data = philo->data;
	while (1)
	{
		gettimeofday(&current_time, NULL);
		sem_wait(data->meal_sem);
		last_meal_time_ms = (philo->last_meal_time.tv_sec * 1000)
			+ (philo->last_meal_time.tv_usec / 1000);
		current_time_ms = (current_time.tv_sec * 1000) + (current_time.tv_usec
				/ 1000);
		sem_post(data->meal_sem);
		if ((current_time_ms - last_meal_time_ms) >= data->time_to_die)
		{
			sem_wait(data->death_sem);
			data->is_dead = 1;
			printf("died");
			sem_post(data->death_sem);
			exit(1);
		}
		if (data->must_eat_count > 0
			&& philo->meals_eaten >= data->must_eat_count)
			{
				sem_wait(philo->data->print_sem);
				sem_wait(data->death_sem);
				data->is_dead = 1;
				sem_post(data->death_sem);
				printf("all philosophers have finished their meals\n");
				sem_post(philo->data->print_sem);
				exit(0);
			}
		print_status(philo, "is thinking");
		if (philo->id % 2 == 0)
			usleep(500);
		sem_wait(data->forks);
		sem_wait(data->forks);
		print_status(philo, "is eating");
		usleep(data->time_to_eat * 1000);
		sem_wait(data->meal_sem);
		philo->last_meal_time = get_current_time();
		philo->meals_eaten++;
		sem_post(data->meal_sem);
		sem_post(data->forks);
		sem_post(data->forks);
		print_status(philo, "is sleeping");
		usleep(data->time_to_sleep * 1000);
	}
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
		sem_wait(philo->data->print_sem);
		printf("%lld %d %s\n", timestamp, philo->id, status);
		sem_post(philo->data->print_sem);
	}
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
