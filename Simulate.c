/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simulate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/04 22:08:02 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int check_if_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_mutex);
	if (!philo->data->dead)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		return 0;
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	return 1;
}
void check_print(t_philo *philo, int i)
{
    long timestamp;

    pthread_mutex_lock(&philo->data->print_mutex);
    pthread_mutex_lock(&philo->data->death_mutex);

    timestamp = get_current_time() - ((philo->data->start_time.tv_sec * 1000) + (philo->data->start_time.tv_usec / 1000)); 

    if (i == 5)
    {
        if (!philo->data->dead)
        {
            philo->data->dead = 1;
            printf("%ld %d died\n", timestamp, philo->id);
        }
    }
    else if (!philo->data->dead)
    {
        if (i == 0)
            printf("%ld %d is sleeping\n", timestamp, philo->id);
        else if (i == 1)
            printf("%ld %d is thinking\n", timestamp, philo->id);
        else if (i == 2)
            printf("%ld %d is eating\n", timestamp, philo->id);
        else if (i == 3)
            printf("%ld %d has finished eating\n", timestamp, philo->id);
        else if (i == 4)
            printf("%ld %d has taken a fork\n", timestamp, philo->id);
    }

    pthread_mutex_unlock(&philo->data->death_mutex);
    pthread_mutex_unlock(&philo->data->print_mutex);
}

void	philo_sleep(t_philo *philo)
{
	long	start_time;

	start_time = get_current_time();
	if (!check_if_dead(philo))
		check_print(philo, 0);
	while ((get_current_time() - start_time < philo->data->time_to_sleep
		&& !check_if_dead(philo)))
	{
		usleep(100);
	}
}
void	think(t_philo *philo)
{
	if (!check_if_dead(philo))
	{
		check_print(philo, 1);
		//usleep(100);
	}
}
void	eat(t_philo *philo)
{
	long	start_time;

	if (!check_if_dead(philo))
		check_print(philo, 2);
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = get_current_time();
	pthread_mutex_unlock(&philo->data->meal_mutex);
	start_time = get_current_time();
	while ((get_current_time() - start_time < philo->data->time_to_eat)
		&& !check_if_dead(philo))
	{
		usleep(100);
	}
	if (!check_if_dead(philo))
	{
		//check_print(philo, 3);
		pthread_mutex_lock(&philo->data->meal_mutex);
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->data->meal_mutex);
	}
}

void take_forks(t_philo *philo)
{
    if (!philo || !philo->data)
        return;
        
    int first_fork = philo->id - 1;
    int second_fork = philo->id % philo->data->num_philos;

    if (first_fork > second_fork)
    {
        int temp = first_fork;
        first_fork = second_fork;
        second_fork = temp;
    }

    philo->has_first_fork = 0;
    philo->has_second_fork = 0;
    
    if (!check_if_dead(philo))
    {
        pthread_mutex_lock(&philo->data->forks[first_fork]);
		check_print(philo, 4);
        philo->has_first_fork = 1;

        if (!check_if_dead(philo))
        {
            pthread_mutex_lock(&philo->data->forks[second_fork]);
			check_print(philo, 4);
            philo->has_second_fork = 1;
        }
        else 
        {
            pthread_mutex_unlock(&philo->data->forks[first_fork]);
            philo->has_first_fork = 0;
        }
    }
}

void release_forks(t_philo *philo)
{
    if (!philo || !philo->data)
        return;
        
    int first_fork = philo->id - 1;
    int second_fork = philo->id % philo->data->num_philos;

    if (first_fork > second_fork)
    {
        int temp = first_fork;
        first_fork = second_fork;
        second_fork = temp;
    }

    if (philo->has_second_fork)
        pthread_mutex_unlock(&philo->data->forks[second_fork]);
    if (philo->has_first_fork)
        pthread_mutex_unlock(&philo->data->forks[first_fork]);
}

