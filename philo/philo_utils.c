/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:19:34 by nmonzon           #+#    #+#             */
/*   Updated: 2024/12/11 17:01:41 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	free_resources(t_seat *table, int n)
{
	t_seat	*current;
	t_seat	*temp;
	int		i;

	current = table;
	i = 0;
	while (i++ < n)
	{
		temp = current->next;
		pthread_mutex_destroy(&current->death_mutex);
		pthread_mutex_destroy(&current->start_mutex);
		free(current->philo);
		current->philo = NULL;
		free(current);
		current = NULL;
		current = temp;
	}
}
