/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 20:55:41 by aelsayed          #+#    #+#             */
/*   Updated: 2025/06/03 21:47:18 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

void	print_state(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->table->print_lock);
	pthread_mutex_lock(&philo->table->death_lock);
	if (philo->table->someone_died == FALSE)
		printf("%ld %d %s\n", get_time() - philo->table->start_time, philo->id, msg);
	pthread_mutex_unlock(&philo->table->death_lock);
	pthread_mutex_unlock(&philo->table->print_lock);
}

void	leave_forks(t_philo *p)
{
	if (p->id % 2 == 0)
	{
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
	}
	else
	{
		pthread_mutex_unlock(p->right_fork);
		pthread_mutex_unlock(p->left_fork);
	}
}
int	take_forks(t_philo *p)
{
	if (is_dead_or_full(p))
		return (FALSE);
	if (p->id % 2 != 0)
	{
		pthread_mutex_lock(p->left_fork);
		print_state(p, FORK);
		pthread_mutex_lock(p->right_fork);
		print_state(p, FORK);
	}
	else
	{
		pthread_mutex_lock(p->right_fork);
		print_state(p, FORK);
		pthread_mutex_lock(p->left_fork);
		print_state(p, FORK);
	}
	if (is_dead_or_full(p))
	{
		leave_forks(p);
		return (FALSE);
	}
	return (TRUE);
}

int	process_eating(t_philo *p)
{
	if (is_dead_or_full(p))
	{
		leave_forks(p);
		return (FALSE);
	}
	print_state(p, EAT);
	pthread_mutex_lock(&p->table->meal_check);
	p->last_meal_time = get_time();
	p->meals_eaten++;
	pthread_mutex_unlock(&p->table->meal_check);
	usleep(p->table->time_to_eat * 1000);
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_unlock(p->right_fork);
	return (TRUE);
}

int	process_sleeping(t_philo *p)
{
	if (is_dead_or_full(p))
		return (FALSE);
	print_state(p, SLEEP);
	usleep(p->table->time_to_sleep * 1000);
	return (TRUE);
}

int	process_thinking(t_philo *p)
{
	if (is_dead_or_full(p))	
		return (FALSE);
	print_state(p, THINK);
	return (TRUE);
}
