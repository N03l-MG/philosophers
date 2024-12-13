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

static bool	death_check(t_routine_args	*args);
static void	die(t_philo *philo, t_seat *seat, long current_time, int philo_n);
static void	eat(t_philo *philo, long current_time);
static void	think(t_routine_args *args, t_philo *philo);

void	*philo_routine(void *arg)
{
	t_routine_args	*args;
	t_philo			*philo;

	args = (t_routine_args *)arg;
	philo = args->philo;
	while (philo->eat_counter != 0)
	{
		if (death_check(args))
			return (NULL);
		log_action(args->start_time, philo, THINK);
		eat(philo, args->start_time);
		if (death_check(args))
			return (NULL);
		log_action(args->start_time, philo, SLEEP);
		usleep(philo->time_to_sleep * 1000);
		if (death_check(args))
			return (NULL);
		think(args, philo);
		if (death_check(args))
			return (NULL);
	}
	free(args);
	return (NULL);
}

static bool	death_check(t_routine_args	*args)
{
	long	current_time;
	t_philo	*philo;
	t_seat	*seat;
	int		i;

	philo = args->philo;
	seat = philo->seat;
	current_time = current_time_ms();
	pthread_mutex_lock(&seat->death_mutex);
	i = -1;
	while (++i < args->philo_n)
	{
		if (seat->has_died)
			return (true);
		seat = seat->next;
	}
	pthread_mutex_unlock(&seat->death_mutex);
	if (current_time - philo->last_meal_time >= philo->time_to_die)
	{
		die(philo, seat, args->start_time, args->philo_n);
		free(args);
		return (true);
	}
	return (false);
}

static void	die(t_philo *philo, t_seat *seat, long start_time, int philo_n)
{
	int	i;

	i = -1;
	pthread_mutex_lock(&seat->death_mutex);
	log_action(start_time, philo, DIE);
	while (++i < philo_n)
	{
		if (!seat->has_died)
			seat->has_died = true;
		seat = seat->next;
	}
	pthread_mutex_unlock(&seat->death_mutex);
	return ;
}

static void	eat(t_philo *philo, long start_time)
{
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

static void	think(t_routine_args *args, t_philo *philo)
{
	long		current_time;
	long		think_time;

	current_time = current_time_ms();
	think_time = philo->time_to_die
		- (current_time - philo->last_meal_time) - philo->time_to_eat;
	if (think_time > 0)
	{
		log_action(args->start_time, philo, THINK);
		usleep(think_time * 1000);
	}
	else
		log_action(args->start_time, philo, THINK);
}
