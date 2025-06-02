/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:42:00 by aelsayed          #+#    #+#             */
/*   Updated: 2025/06/02 19:51:08 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_state(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->table->print_lock);
	pthread_mutex_lock(&philo->table->death_lock);
	if (philo->table->someone_died == FALSE)
		printf("%ld %d %s\n", get_time() - philo->table->start_time, philo->id, msg);
	pthread_mutex_unlock(&philo->table->death_lock);
	pthread_mutex_unlock(&philo->table->print_lock);
}
void	check_death(t_table *t)
{
	int	i;

	i = 0;
	while (i < t->nb_philo)
	{
		pthread_mutex_lock(&t->meal_check);
		if (get_time() - t->philos[i].last_meal_time > t->time_to_die && !t->someone_died)
		{
			t->someone_died = TRUE;
			pthread_mutex_unlock(&t->meal_check);
			pthread_mutex_lock(&t->print_lock);
			printf("%ld %d died\n", get_time() - t->start_time, t->philos[i].id);
			pthread_mutex_unlock(&t->print_lock);
			return ;
		}
		if (t->meals_required != -1 && t->philos[i].meals_eaten >= t->meals_required)
			t->all_full++;
		pthread_mutex_unlock(&t->meal_check);
	}
	if (t->someone_died || (t->meals_required != -1 && t->all_full == t->nb_philo))
	{
		pthread_mutex_lock(&t->death_lock);
		t->someone_died = TRUE;
		return ((void)pthread_mutex_unlock(&t->death_lock));
	}
}
void	*monitor(void *table)
{
	t_table	*t;
	
	t = (t_table *)table;
	while (TRUE)
	{
		t->all_full = 0;
		pthread_mutex_lock(&t->meal_check);
		if (t->someone_died || t->all_full == t->nb_philo)
		{
			pthread_mutex_unlock(&t->meal_check);
			break;
		}
		pthread_mutex_unlock(&t->meal_check);
		check_death(t);
		usleep(1000);
	}
	return (NULL);
}

int	is_dead_or_full(t_philo *p)
{
	int	res;

	pthread_mutex_lock(&p->table->meal_check);
	res = (p->table->someone_died || p->table->all_full == p->table->nb_philo);
	pthread_mutex_unlock(&p->table->meal_check);
	return (res);
}

int	take_forks(t_philo *p)
{
	if (is_dead_or_full(p))
		return (FALSE);
	pthread_mutex_lock(p->left_fork);
	print_state(p, FORK);
	if (is_dead_or_full(p))
	{
		pthread_mutex_unlock(p->left_fork);
		return (FALSE);
	}
	pthread_mutex_lock(p->right_fork);
	print_state(p, FORK);
	if (is_dead_or_full(p))
	{
		pthread_mutex_unlock(p->right_fork);
		pthread_mutex_unlock(p->left_fork);
		return (FALSE);
	}
	return (TRUE);
}

void	*routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->id % 2 == 0)
		usleep(100);
	while (TRUE)
	{
		pthread_mutex_lock(&p->table->death_lock);
		if (p->table->someone_died)
		{
			pthread_mutex_unlock(&p->table->death_lock);
			break ;
		}
		if (is_dead_or_full(p))
			return (NULL);
		if (!take_forks(p))
			return (NULL);
		if (is_dead_or_full(p))
		{
			pthread_mutex_unlock(p->left_fork);
			pthread_mutex_unlock(p->right_fork);
			return (NULL);
		}
		print_state(p, EAT);
		//
		//
		//
		//
		//
		//
		//
		//
		//
		//
		//
		//
		//
		//
		//
		//
		//
		//
		pthread_mutex_unlock(&p->table->death_lock);
		// take the two forks
		pthread_mutex_lock(p->left_fork);
		print_state(p, FORK);
		pthread_mutex_lock(p->right_fork);
		print_state(p, FORK);
		// start eating
		print_state(p, EAT);
		pthread_mutex_lock(&p->meal_lock);
		// save lat meal time and increment the num of meals eaten
		p->last_meal_time = get_time();
		p->meals_eaten++;
		pthread_mutex_unlock(&p->meal_lock);
		// sleep the thread time_to_eat easy malna
		usleep(p->table->time_to_eat * 1000);
		// put the forks down
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);
		// start sleeping after eating (9ayloola)
		print_state(p, SLEEP);
		usleep(p->table->time_to_sleep * 1000);
		// then start thinking until he finds a fork to eat again (i guess)
		print_state(p, THINK);
		// i am adding the little usleep to not have excessive loop aggression over the threads
		// i will later why is this even a problem
		usleep(100);
	}
	return (NULL);
}
