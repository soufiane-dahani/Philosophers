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

void *monitor_dead(void *arg)
{
    t_data *data = (t_data *)arg;
    int i;
    int finished_meals;

    while (1)
    {
        usleep(100);

        finished_meals = 0;

        i = 0;
        while (i < data->num_philos)
        {
            pthread_mutex_lock(&data->meal_mutex);
            long time_since_last_meal = get_current_time() - data->philos[i].last_meal_time;
            pthread_mutex_unlock(&data->meal_mutex);

            pthread_mutex_lock(&data->death_mutex);
            if (time_since_last_meal > data->time_to_die)
            {
                if (!data->dead)
                {
                    data->dead = 1;
                    pthread_mutex_unlock(&data->death_mutex);
                    printf("Philo %d has died\n", data->philos[i].id);
                    return (NULL);
                }
                pthread_mutex_unlock(&data->death_mutex);
            }
            else
            {
                pthread_mutex_unlock(&data->death_mutex);
            }

            pthread_mutex_lock(&data->meal_mutex);
            if (data->must_eat_count > 0 && data->philos[i].meals_eaten >= data->must_eat_count)
                finished_meals++;
            pthread_mutex_unlock(&data->meal_mutex);

            i++;
        }

        if (data->must_eat_count > 0 && finished_meals == data->num_philos)
        {
            pthread_mutex_lock(&data->death_mutex);
            data->dead = 1;
            printf("all philosophers have finished their meals\n");
            pthread_mutex_unlock(&data->death_mutex);
            return (NULL);
        }
    }
    return (NULL);
}



void define_the_thread(t_data *data)
{
    int i;
    pthread_t monitor_thread;

    i = 0;
    while (i < data->num_philos)
    {
        pthread_create(&data->philos[i].thread, NULL, philosopher_routine,
                       &data->philos[i]);
        i++;
    }


    pthread_create(&monitor_thread, NULL, monitor_dead, data);

    i = 0;
    while (i < data->num_philos)
    {
        pthread_join(data->philos[i].thread, NULL);
        i++;
    }

    pthread_join(monitor_thread, NULL);
}



long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
