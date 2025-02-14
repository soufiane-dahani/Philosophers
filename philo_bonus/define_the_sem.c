/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define_the_sem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/14 18:43:05 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_meals(t_philo *philo)
{
	if (!philo || !philo->data || !philo->data->meal_sem)
		return (0);
	sem_wait(philo->data->meal_sem);
	if (philo->data->m->__align == philo->data->num_philos)
	{
		sem_wait(philo->data->print_sem);
		if (philo->data->d->__align != 1)
		{
			philo->data->d->__align = 1;
			printf("all philosophers have finished their meals\n");
		}
		sem_post(philo->data->print_sem);
		sem_post(philo->data->meal_sem);
		return (1);
	}
	sem_post(philo->data->meal_sem);
	return (0);
}

static long long	get_time_in_ms(struct timeval time)
{
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

static int	handle_philosopher_death(t_philo *philo, long long current_time_ms)
{
	long long	timestamp;

	sem_wait(philo->data->death_sem);
	if (philo->data->d->__align != 1)
	{
		philo->data->d->__align = 1;
		sem_post(philo->data->death_sem);
		timestamp = current_time_ms - get_time_in_ms(philo->data->start_time);
		sem_wait(philo->data->print_sem);
		printf("%lld %d died\n", timestamp, philo->id);
		sem_post(philo->data->print_sem);
		return (1);
	}
	sem_post(philo->data->death_sem);
	return (0);
}

static int	check_philosopher_death(t_philo *philo, struct timeval current_time)
{
	long long	last_meal_time_ms;
	long long	current_time_ms;

	if (!philo || !philo->data || !philo->data->meal_sem)
		return (0);
	sem_wait(philo->data->meal_sem);
	last_meal_time_ms = get_time_in_ms(philo->last_meal_time);
	sem_post(philo->data->meal_sem);
	current_time_ms = get_time_in_ms(current_time);
	if ((current_time_ms - last_meal_time_ms) >= philo->data->time_to_die)
		return (handle_philosopher_death(philo, current_time_ms));
	return (0);
}

void	*monitor_death(void *arg)
{
	t_philo			*philo;
	struct timeval	current_time;

	philo = (t_philo *)arg;
	while (1)
	{
		if (check_meals(philo))
			break ;
		gettimeofday(&current_time, NULL);
		if (check_philosopher_death(philo, current_time))
			break ;
		sem_wait(philo->data->death_sem);
		if (philo->data->d->__align == 1)
		{
			sem_post(philo->data->death_sem);
			break ;
		}
		sem_post(philo->data->death_sem);
		usleep(1000);
	}
	return (NULL);
}
