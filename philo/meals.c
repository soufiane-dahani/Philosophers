/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meals.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/09 15:02:42 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_meals(t_data *data)
{
	unsigned int	i;
	unsigned int	meals;

	i = 0;
	meals = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		if (data->philos[i].meals_eaten >= data->must_eat_count)
			meals++;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		i++;
	}
	if (data->num_philos == meals)
	{
		pthread_mutex_lock(&data->print_mutex);
		pthread_mutex_lock(&data->death_mutex);
		data->dead = 1;
		printf("all philosophers have finished their meals\n");
		pthread_mutex_unlock(&data->death_mutex);
		pthread_mutex_unlock(&data->print_mutex);
		return (1);
	}
	return (0);
}
