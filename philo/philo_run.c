/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_run.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:15:05 by nmonzon           #+#    #+#             */
/*   Updated: 2024/12/10 19:19:43 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	die(t_philo *philo, t_seat *seat, long current_time);
static void	eat(t_philo *philo, long current_time);
static void	log_action(long start_time, t_philo *philo, t_action action);

void	monitoring(t_seat *table, pthread_t *threads, int n)
{
	t_seat	*current;
	int		i;
	int		j;

	while (true)
	{
		current = table;
		i = -1;
		j = -1;
		while (++i < n)
		{
			pthread_mutex_lock(&current->death_mutex);
			if (current->has_died)
			{
				pthread_mutex_unlock(&current->death_mutex);
				while (++j < n)
					pthread_join(threads[j], NULL);
				free_resources(table, n);
				return ;
			}
			pthread_mutex_unlock(&current->death_mutex);
			current = current->next;
		}
	}
}

void	*philo_routine(void *arg) // TODO: fix god forsaken norm for the billionth time
{
	t_routine_args	*args = (t_routine_args *)arg;
	t_philo			*philo = args->philo;
	t_seat			*seat = philo->seat;
	long			start_time = args->start_time;
	long			current_time;

	while (philo->eat_counter != 0)
	{
		pthread_mutex_lock(&seat->death_mutex);
		if (seat->has_died)
		{
			pthread_mutex_unlock(&seat->death_mutex);
			free(args);
			return (NULL);
		}
		pthread_mutex_unlock(&seat->death_mutex);
		current_time = current_time_ms();
		if (current_time - philo->last_meal_time > philo->time_to_die)
		{
			die(philo, seat, start_time);
			free(args);
			return (NULL);
		}
		log_action(start_time, philo, THINK);
		eat(philo, start_time);
		log_action(start_time, philo, SLEEP);
		usleep(philo->time_to_sleep * 1000);
	}
	free(args);
	return (NULL);
}

static void	die(t_philo *philo, t_seat *seat, long start_time)
{
	pthread_mutex_lock(&seat->death_mutex);
	if (!seat->has_died)
	{
		seat->has_died = true;
		log_action(start_time, philo, DIE);
	}
	pthread_mutex_unlock(&seat->death_mutex);
	pthread_exit(NULL);
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

static void	log_action(long start_time, t_philo *philo, t_action action)
{
	long	timestamp;	

	timestamp = current_time_ms() - start_time;
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
