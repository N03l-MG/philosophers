/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_run.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:15:05 by nmonzon           #+#    #+#             */
/*   Updated: 2024/12/11 17:08:10 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat(t_philo *philo, long current_time, t_routine_args *args);

void	*philo_routine(void *arg)
{
	t_routine_args	*args;
	t_philo			*philo;

	args = (t_routine_args *)arg;
	philo = args->philo;
	if (philo->philo_id % 2 == 0)
		eat(philo, args->start_time, args);
	while (philo->eat_counter != 0)
	{
		log_action(args->start_time, philo, SLEEP);
		usleep(philo->time_to_sleep * 1000);
		log_action(args->start_time, philo, THINK);
		eat(philo, args->start_time, args);
	}
	log_action(args->start_time, philo, THINK);
	return (NULL);
}

static void	eat(t_philo *philo, long start_time, t_routine_args *args)
{
	if (philo->right_fork == philo->left_fork)
		return ;
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		log_action(start_time, philo, FORK);
		pthread_mutex_lock(philo->right_fork);
		log_action(start_time, philo, FORK);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		log_action(start_time, philo, FORK);
		pthread_mutex_lock(philo->left_fork);
		log_action(start_time, philo, FORK);
	}
	log_action(start_time, philo, EAT);
	philo->last_meal_time = current_time_ms();
	usleep(philo->time_to_eat * 1000);
	if (philo->eat_counter > 0)
		philo->eat_counter--;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}
