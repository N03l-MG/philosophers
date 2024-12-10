/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_run.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmonzon <nmonzon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 15:15:05 by nmonzon           #+#    #+#             */
/*   Updated: 2024/12/10 17:20:26 by nmonzon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	die(t_philo *philo, t_seat *seat, long current_time);
static void	eat(t_philo *philo, long current_time);

void	monitoring(t_seat *table, t_seat *current, pthread_t *threads, int n)
{
	int	i;
	int	j;

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
				pthread_cancel(threads[j]);
			j = -1;
			while (++j < n)
				pthread_join(threads[j], NULL);
			free(threads);
			return ;
		}
		pthread_mutex_unlock(&current->death_mutex);
		current = current->next;
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_seat	*seat;
	long	current_time;

	philo = (t_philo *)arg;
	seat = philo->seat;
	while (philo->eat_counter != 0)
	{
		pthread_mutex_lock(&seat->death_mutex);
		if (seat->has_died)
		{
			pthread_mutex_unlock(&seat->death_mutex);
			pthread_exit(NULL);
		}
		pthread_mutex_unlock(&seat->death_mutex);
		current_time = current_time_ms();
		if (current_time - philo->last_meal_time > philo->time_to_die)
			die(philo, seat, current_time);
		printf("%ld %d is thinking\n", current_time, philo->philo_id);
		eat(philo, current_time);
		current_time = current_time_ms();
		printf("%ld %d is sleeping\n", current_time, philo->philo_id);
		usleep(philo->time_to_sleep * 1000);
	}
	return (NULL);
}

static void	die(t_philo *philo, t_seat *seat, long current_time)
{
	pthread_mutex_lock(&seat->death_mutex);
	if (!seat->has_died)
	{
		seat->has_died = true;
		printf("%ld %d died\n", current_time, philo->philo_id);
	}
	pthread_mutex_unlock(&seat->death_mutex);
	pthread_exit(NULL);
}

static void	eat(t_philo *philo, long current_time)
{
	if (philo->philo_id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		printf("%ld %d has taken a fork\n", current_time, philo->philo_id);
		pthread_mutex_lock(philo->right_fork);
		printf("%ld %d has taken a fork\n", current_time, philo->philo_id);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		printf("%ld %d has taken a fork\n", current_time, philo->philo_id);
		pthread_mutex_lock(philo->left_fork);
		printf("%ld %d has taken a fork\n", current_time, philo->philo_id);
	}
	current_time = current_time_ms();
	printf("%ld %d is eating\n", current_time, philo->philo_id);
	philo->last_meal_time = current_time;
	usleep(philo->time_to_eat * 1000);
	if (philo->eat_counter > 0)
		philo->eat_counter--;
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

static void	log_action(long start_time, int philo_id, t_action action) // I have a plan...
{
	long	timestamp = current_time_ms() - start_time;
	printf("%ld %d %s\n", timestamp, philo_id, action);
}
