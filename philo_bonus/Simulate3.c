/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Simulate3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/14 18:43:10 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo *philo, const char *status)
{
	struct timeval	current_time;
	long long		timestamp;

	sem_wait(philo->data->death_sem);
	if (philo->data->d->__align == 1)
	{
		sem_post(philo->data->death_sem);
		return ;
	}
	sem_post(philo->data->death_sem);
	gettimeofday(&current_time, NULL);
	timestamp = ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000))
		- ((philo->data->start_time.tv_sec * 1000)
			+ (philo->data->start_time.tv_usec / 1000));
	sem_wait(philo->data->print_sem);
	printf("%lld  %d %s\n", timestamp, philo->id, status);
	sem_post(philo->data->print_sem);
}

void	terminate_processes(t_data *data)
{
	size_t	i;

	if (!data)
		return ;
	if (!data->philos)
		return ;
	i = 0;
	while (i < data->num_philos)
	{
		if (data->philos[i].pid > 0)
			kill(data->philos[i].pid, SIGKILL);
		i++;
	}
	cleanup_all(data);
	exit(0);
}

void	wait_and_sem_close(t_data *data)
{
	int		status;
	pid_t	pid;

	pid = waitpid(-1, &status, 0);
	while (pid > 0)
	{
		if (WIFEXITED(status) || WIFSIGNALED(status))
		{
			pid = waitpid(-1, &status, 0);
			continue ;
		}
	}
	while (wait(NULL) > 0)
		;
	terminate_processes(data);
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
