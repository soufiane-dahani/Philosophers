/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/04 17:01:50 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;
	long	time_since_last_meal;
	long timestamp;
	
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
		timestamp = get_current_time() - ((philo->data->start_time.tv_sec * 1000) + (philo->data->start_time.tv_usec / 1000));
		if (philo->data->num_philos == 1)
		{
			printf("%ld %d is thinking\n", timestamp, philo->id);
			usleep(100);
			printf("%ld %d has taken a fork\n", timestamp, philo->id);
			usleep(1000 * philo->data->time_to_die);
			printf("%ld %d died\n", timestamp, philo->id);
			return (NULL);
		}
		pthread_mutex_lock(&philo->data->death_mutex);
		time_since_last_meal = get_current_time() - philo->last_meal_time;
		if ((time_since_last_meal > philo->data->time_to_die))
		{
			philo->data->dead = 1;
			pthread_mutex_unlock(&philo->data->death_mutex);
			printf("%ld %d died\n", timestamp, philo->id);
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
