/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:05:24 by nmonzon           #+#    #+#             */
/*   Updated: 2024/12/13 18:41:33 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	check_all_finished(t_seat *table, int philo_n);

void	monitoring(t_seat *table, pthread_t *threads, int philo_n)
{
	t_seat	*current;
	int		i;
	int		j;

	while (true)
	{
		current = table;
		i = -1;
		j = -1;
		while (++i < philo_n)
		{
			pthread_mutex_lock(&current->death_mutex);
			if (current->has_died)
			{
				while (++j < philo_n)
					pthread_detach(threads[j]);
				free_resources(table, philo_n);
				return ;
			}
			pthread_mutex_unlock(&current->death_mutex);
			current = current->next;
		}
		if (check_all_finished(table, philo_n))
			return ;
	}
}

static bool	check_all_finished(t_seat *table, int philo_n)
{
	int		i;
	int		finished;
	t_seat	*current;

	current = table;
	finished = 0;
	i = -1;
	while (++i < philo_n)
	{
		if (current->philo->eat_counter == 0)
			finished++;
		current = current->next;
	}
	return (finished == philo_n);
}
