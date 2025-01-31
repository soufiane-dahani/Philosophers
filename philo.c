/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/01/31 12:20:44 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
 
t_data *initialize_data(int num_philos, int time_to_die, int time_to_eat, int time_to_sleep)
{
	t_data	*data;
    int		i;

	i = 0;
	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->num_philos = num_philos;
    data->time_to_die = time_to_die;
    data->time_to_eat = time_to_eat;
    data->time_to_sleep = time_to_sleep;
    data->dead = 0;
	data->philos = malloc(sizeof(t_philo) * num_philos);
    if (!data->philos)
	{
        free(data);
        return (NULL);
    }
	data->forks = malloc(sizeof(pthread_mutex_t) * num_philos);
	if (!data->forks)
		return (free(data->philos), free(data), NULL);
	while (i < num_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->print_mutex, NULL);
    pthread_mutex_init(&data->death_mutex, NULL);
	i = 0;
	while (i < num_philos)
	{
		data->philos[i].id = i + 1;
        data->philos[i].meals_eaten = 0;
        data->philos[i].last_meal_time = 0;
        data->philos[i].data = data;
		i++;
	}
	gettimeofday(&data->start_time, NULL);
	return(data);
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
	free(num);
}
