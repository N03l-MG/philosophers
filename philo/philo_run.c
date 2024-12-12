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
static void	die(t_philo *philo, t_seat *seat, long current_time);
static void	eat(t_philo *philo, long current_time);

void	monitoring(t_seat *table, pthread_t *threads, int n)
{
	t_seat	*current;
	int		i;
	int		j;
	int		finished;

	while (true)
	{
		current = table;
		finished = 0;
		i = -1;
		j = -1;
		while (++i < n)
		{
			pthread_mutex_lock(&current->death_mutex);
			if (current->has_died)
			{
				pthread_mutex_unlock(&current->death_mutex);
				while (++j < n)
					pthread_detach(threads[j]);
				free_resources(table, n);
				return ;
			}
			pthread_mutex_unlock(&current->death_mutex);
			current = current->next;
		}
		i = -1;
		while (++i < n)
		{
			if (current->philo->eat_counter == 0)
				finished++;
		}
		if (finished == n)
			return ;
	}
}

void	*philo_routine(void *arg)
{
	t_routine_args	*args;
	t_philo			*philo;
	long			current_time;
	long			think_time;

	args = (t_routine_args *)arg;
	philo = args->philo;
	log_action(args->start_time, philo, THINK, false);
	pthread_mutex_lock(&philo->seat->start_mutex);
	while (!philo->seat->start)
	{
		pthread_mutex_unlock(&philo->seat->start_mutex);
		usleep(100);
		pthread_mutex_lock(&philo->seat->start_mutex);
	}
	pthread_mutex_unlock(&philo->seat->start_mutex);
	while (philo->eat_counter != 0)
	{
		eat(philo, args->start_time);
		if (death_check(args))
			return (NULL);
		log_action(args->start_time, philo, SLEEP, false);
		usleep(philo->time_to_sleep * 1000);
		if (death_check(args))
			return (NULL);
		current_time = current_time_ms();
		think_time = philo->time_to_die
			- (current_time - philo->last_meal_time) - philo->time_to_eat;
		if (think_time > 0)
		{
			log_action(args->start_time, philo, THINK, false);
			usleep(think_time * 1000);
		}
		else
			log_action(args->start_time, philo, THINK, false);
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
	long	start_time;

	philo = args->philo;
	seat = philo->seat;
	start_time = args->start_time;
	current_time = current_time_ms();
	if (current_time - philo->last_meal_time >= philo->time_to_die)
	{
		die(philo, seat, start_time);
		free(args);
		return (true);
	}
	return (false);
}

static void	die(t_philo *philo, t_seat *seat, long start_time)
{
	pthread_mutex_lock(&seat->death_mutex);
	if (!seat->has_died)
	{
		seat->has_died = true;
		log_action(start_time, philo, DIE, true);
	}
	pthread_mutex_unlock(&seat->death_mutex);
	pthread_exit(NULL);
}

static void	eat(t_philo *philo, long start_time)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		log_action(start_time, philo, FORK, false);
		pthread_mutex_lock(philo->right_fork);
		log_action(start_time, philo, FORK, false);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		log_action(start_time, philo, FORK, false);
		pthread_mutex_lock(philo->left_fork);
		log_action(start_time, philo, FORK, false);
	}
	log_action(start_time, philo, EAT, false);
	philo->last_meal_time = current_time_ms();
	usleep(philo->time_to_eat * 1000);
	if (philo->eat_counter > 0)
		philo->eat_counter--;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}
