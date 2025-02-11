/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define_the_sem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/11 15:16:49 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	terminate_processes(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (data->philos[i].pid > 0)
			kill(data->philos[i].pid, SIGKILL);
		i++;
	}
}

static void	wait_and_sem_close(t_data *data)
{
	int		status;
	pid_t	pid;

	pid = 0;
	while ((pid = waitpid(-1, &status, 0)) > 0)
	{
		if (WIFEXITED(status) || WIFSIGNALED(status))
			continue ;
	}
	terminate_processes(data);
	sem_close(data->forks);
	sem_close(data->print_sem);
	sem_close(data->meal_sem);
	sem_close(data->death_sem);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/meal_check");
	sem_unlink("/death_sem");
}

int	start_simulation(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].pid = fork();
		if (data->philos[i].pid < 0)
		{
			write(2, "Error: fork failed\n", 20);
			terminate_processes(data);
			return (1);
		}
		if (data->philos[i].pid == 0)
		{
			philosopher_lifecycle(&(data->philos[i]));
			exit(0);
		}
		i++;
	}
	wait_and_sem_close(data);
	return (0);
}

void	*monitor_philosophers(void *arg)
{
	struct timeval	current_time;
	long long		last_meal_time_ms;
	long long		current_time_ms;
	long long		timestamp;
	unsigned int	i;
	unsigned int	finished_meals;
	t_data			*data;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		finished_meals = 0;
		while (i < data->num_philos)
		{
			gettimeofday(&current_time, NULL);
			sem_wait(data->meal_sem);
			last_meal_time_ms = (data->philos[i].last_meal_time.tv_sec * 1000)
				+ (data->philos[i].last_meal_time.tv_usec / 1000);
			current_time_ms = (current_time.tv_sec * 1000)
				+ (current_time.tv_usec / 1000);
			sem_post(data->meal_sem);
			timestamp = ((current_time.tv_sec * 1000) + (current_time.tv_usec
						/ 1000)) - ((data->start_time.tv_sec * 1000)
					+ (data->start_time.tv_usec / 1000));
			if ((current_time_ms - last_meal_time_ms) >= data->time_to_die)
			{
				sem_wait(data->death_sem);
				if (data->is_dead == 0)
				{
					data->is_dead = 1;
					sem_wait(data->print_sem);
					printf("%lld %d died\n", timestamp, data->philos[i].id);
					sem_post(data->print_sem);
				}
				sem_post(data->death_sem);
				terminate_processes(data);
				exit(1);
			}
			sem_wait(data->meal_sem);
			if (data->philos[i].meals_eaten >= data->must_eat_count)
				finished_meals++;
			sem_post(data->meal_sem);
			i++;
		}
		if (finished_meals == data->num_philos)
		{
			sem_wait(data->death_sem);
			sem_wait(data->print_sem);
			data->is_dead = 1;
			printf("All philosophers have finished their meals\n");
			sem_post(data->print_sem);
			sem_post(data->death_sem);
			terminate_processes(data);
			exit(0);
		}
		usleep(5000);
	}
}
