/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define_the_thread.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/04 22:08:28 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_routine(void *arg);

void	free_data(t_data *data)
{
	size_t	i;

	for (i = 0; i < data->num_philos; i++)
		pthread_mutex_destroy(&data->forks[i]);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->meal_mutex);
	free(data->forks);
	free(data->philos);
	free(data);
}
int start_simulation(t_data *data)
{
    size_t i;
    pthread_t monitor;

    for (i = 0; i < data->num_philos; i++)
    {
        if (pthread_create(&data->philos[i].thread, NULL, philosopher_routine, &data->philos[i]) != 0)
        {
            write(2, "Error: Failed to create philosopher thread\n", 44);
            return (1);
        }
    }

    if (pthread_create(&monitor, NULL, monitor_death, data) != 0)
    {
        write(2, "Error: Failed to create monitor thread\n", 40);
        return (1);
    }
    
    pthread_detach(monitor);

    for (i = 0; i < data->num_philos; i++)
        pthread_join(data->philos[i].thread, NULL);

    free_data(data);
    return (0);
}



void	print_status(t_philo *philo, const char *status)
{
	struct timeval	current_time;
	long long		timestamp;

	gettimeofday(&current_time, NULL);
	timestamp = ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000)) -
		((philo->data->start_time.tv_sec * 1000)
				+ (philo->data->start_time.tv_usec / 1000));
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->dead)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%lld %d %s\n", timestamp, philo->id, status);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int check_if_dead(t_philo *philo)
{
    pthread_mutex_lock(&philo->data->death_mutex);
    int dead = philo->data->dead;
    pthread_mutex_unlock(&philo->data->death_mutex);
    return dead;
}
void smart_sleep(t_philo *philo, long long duration)
{
    long long start_time = get_current_time_ms();

    while ((get_current_time_ms() - start_time < duration) && !check_if_dead(philo))
    {
        usleep(50);
    }
}



void *philosopher_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;

    while (1)
    {
        if (check_if_dead(philo)) return (NULL);

        print_status(philo, "is thinking");

        pick_forks(philo);
        pthread_mutex_lock(&philo->meal_mutex);
        philo->last_meal_time = get_current_time();
        pthread_mutex_unlock(&philo->meal_mutex);

        print_status(philo, "has taken a fork");
        print_status(philo, "is eating");

        smart_sleep(philo, philo->data->time_to_eat);

        pthread_mutex_lock(&philo->meal_mutex);
        philo->meals_eaten++;
        pthread_mutex_unlock(&philo->meal_mutex);

        release_forks(philo);

        if (check_if_dead(philo)) return (NULL);

        print_status(philo, "is sleeping");
        smart_sleep(philo, philo->data->time_to_sleep);
    }
    return (NULL);
}




void pick_forks(t_philo *philo)
{
    int left_fork = philo->id - 1;
    int right_fork = philo->id % philo->data->num_philos;

    if (philo->id % 2 == 0)
        usleep(200);
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&philo->data->forks[right_fork]);
        pthread_mutex_lock(&philo->data->forks[left_fork]);
    }
    else
    {
        pthread_mutex_lock(&philo->data->forks[left_fork]);
        pthread_mutex_lock(&philo->data->forks[right_fork]);
    }
}



void release_forks(t_philo *philo)
{
    int left_fork = philo->id - 1;
    int right_fork = philo->id % philo->data->num_philos;

    if (philo->id % 2 == 0)
    {
        pthread_mutex_unlock(&philo->data->forks[left_fork]);
        pthread_mutex_unlock(&philo->data->forks[right_fork]);
    }
    else
    {
        pthread_mutex_unlock(&philo->data->forks[right_fork]);
        pthread_mutex_unlock(&philo->data->forks[left_fork]);
    }
}


void *monitor_death(void *arg)
{
    t_data *data = (t_data *)arg;
    size_t i;
    struct timeval current_time;

    while (1)
    {
        for (i = 0; i < data->num_philos; i++)
        {
            gettimeofday(&current_time, NULL);

            pthread_mutex_lock(&data->philos[i].meal_mutex);
            long long last_meal_time_ms = (data->philos[i].last_meal_time.tv_sec * 1000) +
                                          (data->philos[i].last_meal_time.tv_usec / 1000);
            long long current_time_ms = (current_time.tv_sec * 1000) +
                                        (current_time.tv_usec / 1000);
            pthread_mutex_unlock(&data->philos[i].meal_mutex);
            if ((current_time_ms - last_meal_time_ms) >= data->time_to_die + 5)
            {
                print_status(&data->philos[i], "died");
                pthread_mutex_lock(&data->death_mutex);
                data->dead = 1;
                pthread_mutex_unlock(&data->death_mutex);
                return (NULL);
            }
        }
        usleep(2000);
    }
    return (NULL);
}
