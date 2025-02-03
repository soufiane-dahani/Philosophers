/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/03 12:25:05 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;
	long	time_since_last_meal;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->data->dead)
		{
			pthread_mutex_unlock(&philo->data->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
		if (philo->data->num_philos == 1)
		{
			printf("philo %d is thinking\n", philo->id);
			usleep(100);
			printf("Philo %d is trying to take fork %d\n", philo->id, 0);
			usleep(1000 * philo->data->time_to_die);
			printf("Philo %d has died\n", philo->id);
			return (NULL);
		}
		pthread_mutex_lock(&philo->data->death_mutex);
		time_since_last_meal = get_current_time() - philo->last_meal_time;
		if ((time_since_last_meal > philo->data->time_to_die))
		{
			philo->data->dead = 1;
			pthread_mutex_unlock(&philo->data->death_mutex);
			printf("Philo %d has died\n", philo->id);
			return (NULL);
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
		think(philo);
		take_forks(philo);
		eat(philo);
		release_forks(philo);
		philo_sleep(philo);
	}
	return (NULL);
}

int	main(int ac, char const **av)
{
	int		*num;
	int		capacity;
	t_data	*data;
	int		num_philos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;

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
	data = initialize_data(num_philos, time_to_die, time_to_eat, time_to_sleep);
	if (!data)
	{
		write(2, "Memory allocation error\n", 24);
		return (1);
	}
	data->must_eat_count = 2147483648;
	if (capacity == 5)
		data->must_eat_count = num[4];
	define_the_thread(data);
	free(num);
}
