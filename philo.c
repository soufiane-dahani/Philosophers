/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/01 17:48:24 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
 
void define_the_thread(t_data *data)
{
    int i;
    i = 0;
    while (i < data->num_philos)
    {
        pthread_create(&data->philos[i].thread, NULL, philosopher_routine, &data->philos[i]);
        i++;
    }
}
long get_current_time(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
void philo_sleep(t_philo *philo)
{
    long start_time = get_current_time();
    while (get_current_time() - start_time < philo->data->time_to_sleep)
        usleep(100);
}
void think(t_philo *philo)
{
    printf("philo %d is thinking\n", philo->id);
    usleep(500);
}

void take_forks(t_philo *philo)
{
    int left_fork = (philo->id - 1) % philo->data->num_philos;
    int right_fork = philo->id % philo->data->num_philos;

    int first_fork = (left_fork < right_fork) ? left_fork : right_fork;
    int second_fork = (left_fork < right_fork) ? right_fork : left_fork;

    pthread_mutex_lock(&philo->data->forks[first_fork]);
    pthread_mutex_lock(&philo->data->forks[second_fork]);
}
void eat(t_philo *philo)
{
    printf("Philo %d is eating\n", philo->id);
    
    long start_time = get_current_time();
    while (get_current_time() - start_time < philo->data->time_to_eat)
    {
        usleep(100);
    }
    printf("Philo %d has finished eating\n", philo->id);
	philo->meals_eaten++;
}


void release_forks(t_philo *philo)
{
    int left_fork = (philo->id - 1) % philo->data->num_philos;
    int right_fork = philo->id % philo->data->num_philos;

    // Add resource hierarchy to prevent deadlock
    int first_fork = (left_fork < right_fork) ? left_fork : right_fork;
    int second_fork = (left_fork < right_fork) ? right_fork : left_fork;

    pthread_mutex_unlock(&philo->data->forks[first_fork]);

    pthread_mutex_unlock(&philo->data->forks[second_fork]);
}


void *philosopher_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    

    while (!philo->data->dead)
    {
        think(philo);
        take_forks(philo);
        eat(philo);
        release_forks(philo);
        philo_sleep(philo);
    }
    return NULL;
}



int	main(int ac, char const **av)
{
	int	*num;
	int	capacity;
	t_data *data;
	int num_philos;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
	// int must_eat_count;

	if (ac == 1)
	{
		write(2, "Error\n", 6);
		return (1);
	}
	num = check_number(ac, av, &capacity);
	if (!num || (capacity != 4 && capacity != 5))
	{
		write(2, "Error\n", 6);
		return (free(num), 1);
	}
	num_philos = num[0];
	time_to_die = num[1];
	time_to_eat = num[2];
	time_to_sleep = num[3];
	// must_eat_count = -1;
	// if (capacity == 5)
	// 	must_eat_count = num[4];
	data = initialize_data(num_philos, time_to_die, time_to_eat, time_to_sleep);
	if (!data)
	{
		write(2, "Memory allocation error\n", 24);
		return(1);
	}
	define_the_thread(data);
	free(num);
}
