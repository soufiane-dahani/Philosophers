/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define_the_sem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/02/14 18:06:27 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

static void	wait_and_sem_close(t_data *data)
{
	int		status;
	pid_t	pid;

	while ((pid = waitpid(-1, &status, 0)) > 0)
	{
		if (WIFEXITED(status) || WIFSIGNALED(status))
			continue ;
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
static int	check_meals(t_philo *philo)
{
	if (!philo || !philo->data || !philo->data->meal_sem)
		return (0);
	sem_wait(philo->data->meal_sem);
	if (philo->data->m->__align == philo->data->num_philos)
	{
		sem_wait(philo->data->print_sem);
		sem_wait(philo->data->death_sem);
		if (philo->data->d->__align != 1)
		{
			philo->data->d->__align = 1;
			printf("all philosophers have finished their meals\n");
		}
		sem_post(philo->data->death_sem);
		sem_post(philo->data->print_sem);
		sem_post(philo->data->meal_sem);
		return (1);
	}
	sem_post(philo->data->meal_sem);
	return (0);
}

static int	check_philosopher_death(t_philo *philo, struct timeval current_time)
{
	long long	last_meal_time_ms;
	long long	current_time_ms;
	long long	timestamp;

	if (!philo || !philo->data || !philo->data->meal_sem)
		return (0);
	sem_wait(philo->data->meal_sem);
	last_meal_time_ms = (philo->last_meal_time.tv_sec * 1000)
		+ (philo->last_meal_time.tv_usec / 1000);
	sem_post(philo->data->meal_sem);
	current_time_ms = (current_time.tv_sec * 1000) + (current_time.tv_usec
			/ 1000);
	if ((current_time_ms - last_meal_time_ms) >= philo->data->time_to_die)
	{
		sem_wait(philo->data->death_sem);
		if (philo->data->d->__align != 1)
		{
			philo->data->d->__align = 1;
			sem_post(philo->data->death_sem);
			timestamp = current_time_ms - ((philo->data->start_time.tv_sec
						* 1000) + (philo->data->start_time.tv_usec / 1000));
			sem_wait(philo->data->print_sem);
			printf("%lld %d died\n", timestamp, philo->id);
			sem_post(philo->data->print_sem);
			return (1);
		}
		sem_post(philo->data->death_sem);
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
		sem_wait(philo->data->death_sem);
		if (philo->data->d->__align == 1)
		{
			sem_post(philo->data->death_sem);
			break ;
		}
		sem_post(philo->data->death_sem);
		usleep(1000);
	}
	return (NULL);
}
