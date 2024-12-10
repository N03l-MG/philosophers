/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:19:34 by nmonzon           #+#    #+#             */
/*   Updated: 2024/12/10 16:57:05 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;
	int	i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}

t_seat	*ft_lstnew(void *content)
{
	t_seat	*list;

	list = (t_seat *)malloc(sizeof(t_seat));
	if (!list)
		return (NULL);
	list->philo = content;
	list->next = NULL;
	return (list);
}

long	current_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

t_philo	*create_philo(char **args, int *g_and_i, MUTEX *l_fork, t_seat *seat)
{
	t_philo	*philosopher;

	philosopher = malloc(sizeof(t_philo));
	if (!philosopher)
		return (NULL);
	philosopher->philo_id = g_and_i[1];
	philosopher->time_to_die = ft_atoi(args[1]);
	philosopher->time_to_eat = ft_atoi(args[2]);
	philosopher->time_to_sleep = ft_atoi(args[3]);
	philosopher->eat_counter = g_and_i[0];
	philosopher->left_fork = l_fork;
	philosopher->right_fork = NULL;
	philosopher->last_meal_time = current_time_ms();
	philosopher->seat = seat;
	return (philosopher);
}

void	give_forks(t_seat *prev, t_seat *current)
{
	prev->next = current;
	current->philo->right_fork = prev->philo->left_fork;
}
