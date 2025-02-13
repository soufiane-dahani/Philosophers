/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define_the_sem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/13 15:28:46 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	terminate_processes(t_data *data)
{
	size_t	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (data->philos[i].pid > 0)
			kill(data->philos[i].pid, SIGKILL);
		i++;
	}
	sem_close(data->forks);
	sem_close(data->print_sem);
	sem_close(data->meal_sem);
	sem_close(data->death_sem);
	sem_close(data->d);
	sem_close(data->m);
	sem_unlink("/d");
	sem_unlink("/m");
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/meal_check");
	sem_unlink("/death_sem");
	sem_unlink("/last_meal");
	sem_unlink("/meal_count");
	free(data->philos);
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
static int	check_meals(t_philo *philo)
{
	if (philo->data->num_philos == philo->data->m->__align)
	{
		sem_wait(philo->data->print_sem);
		if (philo->data->d->__align != 1)
		{
			sem_wait(philo->data->death_sem);
			philo->data->d->__align = 1;
			printf("all philosophers have finished their meals\n");
			sem_post(philo->data->death_sem);
		}
		terminate_processes(philo->data);
		sem_post(philo->data->print_sem);
		return (1);
	}
	return (0);
}
static int	check_philosopher_death(t_philo *philo, struct timeval current_time)
{
	long long	last_meal_time_ms;
	long long	current_time_ms;
	long long	timestamp;

	last_meal_time_ms = (philo->last_meal_time.tv_sec * 1000)
		+ (philo->last_meal_time.tv_usec / 1000);
	current_time_ms = (current_time.tv_sec * 1000) + (current_time.tv_usec
			/ 1000);
	if ((current_time_ms - last_meal_time_ms) >= philo->data->time_to_die
		&& philo->data->d->__align != 1)
	{
		philo->data->d->__align = 1;
		gettimeofday(&current_time, NULL);
		timestamp = ((current_time.tv_sec * 1000) + (current_time.tv_usec
					/ 1000)) - ((philo->data->start_time.tv_sec * 1000)
				+ (philo->data->start_time.tv_usec / 1000));
		sem_wait(philo->data->print_sem);
		printf("%lld %d %s\n", timestamp, philo->id, "died");
		sem_post(philo->data->print_sem);
		terminate_processes(philo->data);
		return (1);
	}
	return (0);
}

void	*monitor_death(void *arg)
{
	t_philo			*philo;
	struct timeval	current_time;

	philo = (t_philo *)arg;
	while (1)
	{
		if (check_meals(philo))
			break ;
		gettimeofday(&current_time, NULL);
		if (check_philosopher_death(philo, current_time))
			break ;
		if (philo->data->num_philos % 2 != 0)
			usleep(200);
	}
	return (NULL);
}
