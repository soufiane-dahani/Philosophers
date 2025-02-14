/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simulate2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/14 18:37:17 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_if_dead(t_philo *philo)
{
	int	is_dead;

	sem_wait(philo->data->death_sem);
	is_dead = philo->data->d->__align;
	sem_post(philo->data->death_sem);
	return (is_dead);
}

void	cleanup_all(t_data *data)
{
	if (data)
	{
		cleanup_sems(data);
		if (data->philos)
			free(data->philos);
		free(data);
	}
}

int	handle_single_philosopher(t_data *data)
{
	printf("0 1 is thinking\n");
	usleep(data->time_to_die * 1000);
	printf("%d 1 died\n", data->time_to_die);
	cleanup_all(data);
	return (0);
}

int	handle_meal_completion(t_data *data)
{
	printf("all philosophers have finished their meals\n");
	cleanup_all(data);
	return (0);
}

int	initialize_simulation(int ac, char const **av, t_data **data)
{
	int	*num;
	int	capacity;

	num = NULL;
	if (validate_args(ac, av, &capacity, &num))
		return (1);
	*data = setup_data(num, capacity);
	if (!*data)
	{
		write(2, "Memory allocation error\n", 24);
		return (1);
	}
	return (0);
}
