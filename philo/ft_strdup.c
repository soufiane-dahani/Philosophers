/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 10:49:49 by sodahani          #+#    #+#             */
/*   Updated: 2025/01/30 17:49:13 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

char	*ft_strdup(const char *src)
{
	size_t	size;
	char	*dest;

	size = ft_strlen(src);
	dest = (char *)malloc(size * sizeof(char) + 1);
	if (dest == NULL)
		return (0);
	ft_memcpy(dest, src, size);
	dest[size] = '\0';
	return (dest);
}

int	check_all_numbers(char **res)
{
	int	i;

	i = 0;
	while (res[i])
	{
		if (!res[i] || !is_number(res[i]))
		{
			free_string_array(res);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	count_elements(char **res)
{
	int	count;

	count = 0;
	while (res[count])
		count++;
	return (count);
}

int	*convert_to_integers(char **res, int *capacity)
{
	int		*num;
	int		i;
	long	tmp;

	*capacity = count_elements(res);
	num = malloc(*capacity * sizeof(int));
	if (!num)
		return (free_string_array(res), NULL);
	i = 0;
	while (i < *capacity)
	{
		tmp = ft_atoi(res[i]);
		num[i] = (int)tmp;
		free(res[i]);
		i++;
	}
	free(res);
	return (num);
}

int	*check_number(int ac, char const **av, int *capacity)
{
	char	*result;
	char	**res;
	int		*numbers;
	int		i;

	i = -1;
	result = join_av(ac, av);
	if (!result)
		return (NULL);
	while (result[++i])
		if (result[i] == '+' || result[i] == '-')
			if (result[i + 1] == ' ' || result[i + 1] == '\0')
				return (free(result), NULL);
	res = ft_split(result, ' ');
	free(result);
	if (!res || !*res)
		return (NULL);
	if (!check_all_numbers(res))
		return (NULL);
	if (!check_min_max(res))
		return (free_string_array(res), NULL);
	numbers = convert_to_integers(res, capacity);
	return (numbers);
}
