/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:34:41 by sodahani          #+#    #+#             */
/*   Updated: 2025/01/30 16:26:03 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char const **av)
{
	int	*num;
	int	capacity;

	if (ac == 1)
		return (0);
	num = check_number(ac, av, &capacity);
	if (!num || (capacity != 4 && capacity != 5))
	{
		write(2, "Error\n", 6);
		return (free(num), 1);
	}
	free(num);
}
