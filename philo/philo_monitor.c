/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 15:05:24 by nmonzon           #+#    #+#             */
/*   Updated: 2024/12/16 17:26:40 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	check_all_finished(t_seat *table, int philo_n);
static bool	death_check(t_routine_args	*args);
static void	join_all_philos(t_seat *table, pthread_t *threads, int philo_n);

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
			if (death_check(current->args))
			{
				while (++j < philo_n)
					pthread_detach(threads[j]);
				pthread_mutex_unlock(&current->death_mutex);
				return (free_resources(table, philo_n));
			}
			pthread_mutex_unlock(&current->death_mutex);
			current = current->next;
		}
		if (check_all_finished(table, philo_n))
			return (join_all_philos(table, threads, philo_n));
	}
}

static bool	death_check(t_routine_args	*args)
{
	long	current_time;
	t_philo	*philo;
	t_seat	*seat;

	philo = args->philo;
	seat = philo->seat;
	current_time = current_time_ms();
	if (current_time - philo->last_meal_time >= philo->time_to_die)
	{
		log_action(args->start_time, philo, DIE);
		seat->has_died = true;
		free(args);
		return (true);
	}
	return (false);
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

static void	join_all_philos(t_seat *table, pthread_t *threads, int philo_n)
{
	int		i;
	t_seat	*current;

	i = -1;
	current = table;
	while (++i < philo_n)
	{
		pthread_join(threads[i], NULL);
		free(current->args);
		current = current->next;
	}
	free_resources(table, philo_n);
}
