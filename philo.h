/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sodahani <sodahani@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:35:01 by sodahani          #+#    #+#             */
/*   Updated: 2025/01/30 15:58:50 by sodahani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

int		ft_isdigit(int c);
char	**ft_split(char const *s, char c);
size_t	ft_strlen(const char *str);
char	*ft_strjoin(char const *s1, char const *s2);
char	*join_av(int ac, char const **av);
char	*ft_strdup(const char *src);
void	*ft_memcpy(void *dest, const void *src, size_t n);
int		is_number(char *str);
int		check_all_numbers(char **res);
int		*check_number(int ac, char const **av, int *capacity);
int		*convert_to_integers(char **res, int *capacity);
long	ft_atoi(const char *str);
void	free_string_array(char **arr);
int		check_min_max(char **res);

#endif