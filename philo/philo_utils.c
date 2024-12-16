/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:19:34 by nmonzon           #+#    #+#             */
/*   Updated: 2024/12/16 16:28:17 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_philo	*create_philo(char **args, int *g_and_i, MUTEX *l_fork, t_seat *seat)
{
	t_philo	*philosopher;

	philosopher = malloc(sizeof(t_philo));
	if (!philosopher)
		return (NULL);
	philosopher->philo_id = g_and_i[1] + 1;
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
	if (prev)
	{
		prev->next = current;
		current->philo->right_fork = prev->philo->left_fork;
	}
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
		pthread_mutex_destroy(current->philo->left_fork);
		free(current->philo);
		free(current);
		current = NULL;
		current = temp;
	}
}

void	log_action(long start_time, t_philo *philo, t_action action)
{
	long	timestamp;

	timestamp = current_time_ms() - start_time;
	if (philo->seat->has_died)
		return ;
	if (action == DIE)
		printf("%ld %d died\n", timestamp, philo->philo_id);
	else if (action == FORK)
		printf("%ld %d has taken a fork\n", timestamp, philo->philo_id);
	else if (action == EAT)
		printf("%ld %d is eating\n", timestamp, philo->philo_id);
	else if (action == THINK)
		printf("%ld %d is thinking\n", timestamp, philo->philo_id);
	else if (action == SLEEP)
		printf("%ld %d is sleeping\n", timestamp, philo->philo_id);
}
