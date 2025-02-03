/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define_the_thread.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/02 17:36:49 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	define_the_thread(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_create(&data->philos[i].thread, NULL, philosopher_routine,
				&data->philos[i]);
		i++;
	}
	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

long	get_current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
