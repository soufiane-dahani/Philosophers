/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/04 21:46:30 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char const **av)
{
	int				*num;
	int				capacity;
	t_data			*data;
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	unsigned int	must_eat_count;

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
	must_eat_count = (capacity == 5) ? num[4] : 2147483647;
	data = initialize_data(num_philos, time_to_die, time_to_eat, time_to_sleep,
			must_eat_count);
	if (!data)
	{
		write(2, "Memory allocation error\n", 24);
		return (free(num), 1);
	}
	if (data->must_eat_count == 0)
	{
		printf("all philosophers have finished their meals\n");
		free(num);
		return (0);
	}
	start_simulation(data);
}
