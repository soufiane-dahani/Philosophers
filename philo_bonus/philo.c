/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/14 18:44:51 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	validate_args(int ac, char const **av, int *capacity, int **num)
{
	if (ac == 1)
		return (write(2, "Error\n", 6), 1);
	*num = check_number(ac, av, capacity);
	if (!(*num) || (*capacity != 4 && *capacity != 5))
	{
		write(2, "Error\n", 6);
		if (*num)
			free(*num);
		return (1);
	}
	if (*num[0] > 200)
	{
		write(2, "Please enter fewer than 201 philos\n", 35);
		if (*num)
			free(*num);
		return (1);
	}
	return (0);
}

t_data	*setup_data(int *num, int capacity)
{
	t_init	init;

	init.num_philos = num[0];
	init.time_to_die = num[1];
	init.time_to_eat = num[2];
	init.time_to_sleep = num[3];
	if (capacity == 5)
		init.must_eat_count = num[4];
	else
		init.must_eat_count = 2147483647;
	free(num);
	return (initialize_data(init));
}

long long	get_current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (((long long)tv.tv_sec * 1000LL) + ((long long)tv.tv_usec / 1000LL));
}

struct timeval	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv);
}

int	main(int ac, char const **av)
{
	t_data	*data;

	if (initialize_simulation(ac, av, &data))
		return (1);
	if (data->num_philos == 1)
		return (handle_single_philosopher(data));
	if (data->must_eat_count == 0)
		return (handle_meal_completion(data));
	if (start_simulation(data) != 0)
	{
		cleanup_all(data);
		return (1);
	}
	terminate_processes(data);
	cleanup_all(data);
	return (0);
}
