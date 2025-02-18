/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/15 10:21:07 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	init_forks(t_data *data)
{
	sem_unlink("/forks");
	data->forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, data->num_philos);
	if (data->forks == SEM_FAILED)
	{
		write(2, "Error: sem_open failed\n", 23);
		return (1);
	}
	return (0);
}

static int	init_global_sems(t_data *data)
{
	sem_unlink("/print");
	sem_unlink("/meal_check");
	sem_unlink("/death_sem");
	sem_unlink("/d");
	sem_unlink("/m");
	data->d = sem_open("/d", O_CREAT | O_EXCL, 0644, 1);
	data->m = sem_open("/m", O_CREAT | O_EXCL, 0644, 1);
	data->print_sem = sem_open("/print", O_CREAT | O_EXCL, 0644, 1);
	data->meal_sem = sem_open("/meal_check", O_CREAT | O_EXCL, 0644, 1);
	data->death_sem = sem_open("/death_sem", O_CREAT | O_EXCL, 0644, 1);
	if (data->print_sem == SEM_FAILED || data->meal_sem == SEM_FAILED
		|| data->death_sem == SEM_FAILED || data->d == SEM_FAILED
		|| data->m == SEM_FAILED)
	{
		write(2, "Error: sem_open failed\n", 23);
		return (1);
	}
	data->d->__align = 0;
	data->m->__align = 0;
	return (0);
}

static void	init_philosophers(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = get_current_time();
		data->philos[i].data = data;
		data->philos[i].has_finished_meals = false;
		i++;
	}
}

static int	init_sem_and_philos(t_data *data)
{
	if (init_forks(data))
		return (1);
	if (init_global_sems(data))
		return (1);
	init_philosophers(data);
	return (0);
}

t_data	*initialize_data(t_init init)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	memset(data, 0, sizeof(t_data));
	data->num_philos = init.num_philos;
	data->time_to_die = init.time_to_die;
	data->time_to_eat = init.time_to_eat;
	data->time_to_sleep = init.time_to_sleep;
	data->must_eat_count = init.must_eat_count;
	data->is_dead = 0;
	data->philos = malloc(sizeof(t_philo) * init.num_philos);
	if (!data->philos)
		return (free(data), NULL);
	memset(data->philos, 0, sizeof(t_philo) * init.num_philos);
	if (init_sem_and_philos(data))
	{
		cleanup_sems(data);
		free(data->philos);
		free(data);
		return (NULL);
	}
	gettimeofday(&data->start_time, NULL);
	return (data);
}
