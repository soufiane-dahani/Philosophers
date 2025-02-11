/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define_the_sem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/11 17:53:59 by sodahani         ###   ########.fr       */
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
	exit(1);
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
	t_philo			*philo;
	t_data			*data;
	struct timeval	current_time;

	philo = (t_philo *)arg;
	data = philo->data;
	long long last_meal_time_ms, current_time_ms, timestamp;
	while (1)
	{
		gettimeofday(&current_time, NULL);
		sem_wait(data->last_meal_sem);
		last_meal_time_ms = (philo->last_meal_time.tv_sec * 1000)
			+ (philo->last_meal_time.tv_usec / 1000);
		sem_post(data->last_meal_sem);
		current_time_ms = (current_time.tv_sec * 1000) + (current_time.tv_usec
				/ 1000);
		timestamp = ((current_time.tv_sec * 1000) + (current_time.tv_usec
					/ 1000)) - ((data->start_time.tv_sec * 1000)
				+ (data->start_time.tv_usec / 1000));
		sem_wait(data->death_sem);
		if ((current_time_ms - last_meal_time_ms) >= data->time_to_die
			&& data->is_dead == 0)
		{
			data->is_dead = 1;
			sem_wait(data->print_sem);
			printf("%lld %d died\n", timestamp, philo->id);
			sem_post(data->print_sem);
			sem_post(data->death_sem);
			terminate_processes(data);
			exit(1);
		}
		sem_post(data->death_sem);
		sem_wait(data->meal_count_sem);
		if (data->meals_finished == data->num_philos)
		{
			sem_wait(data->print_sem);
			printf("All philosophers have eaten %d meals\n",
					data->must_eat_count);
			sem_post(data->print_sem);
			sem_post(data->meal_count_sem);
			terminate_processes(data);
			exit(0);
		}
		sem_post(data->meal_count_sem);
		usleep(1000);
	}
}
