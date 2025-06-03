/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 21:01:16 by aelsayed          #+#    #+#             */
/*   Updated: 2025/06/03 19:24:00 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	destroy_table(t_table *table, int count, int philo_count)
{
	int i;
	int	end;

	i = 0;
	while (i < count)
		pthread_mutex_destroy(&table->forks[i++]);
	free(table->forks);
	if (table->cleanup >= 1)
		pthread_mutex_destroy(&table->print_lock);
	if (table->cleanup >= 2)
		pthread_mutex_destroy(&table->death_lock);
	// if (table->cleanup >= 3)
	if (table->cleanup >= 3 && table->philos)
		free(table->philos);
	i = 0;
	if (table->cleanup >= 5)
	{
		end = (table->cleanup > 5) * \
			table->nb_philo + (table->cleanup == 5) + philo_count;
		while (i < philo_count)
			pthread_mutex_destroy(&table->philos[i++].meal_lock);
		i = 0;
		while (i < philo_count)
			pthread_mutex_destroy(&table->philos[i++].eat_lock);
	}
	i = 0;
	if (table->cleanup >= 6)
		while (i < philo_count)
			pthread_join(table->philos[i++].thread_id, NULL);
	return (FALSE);
}

int	init_philos	(t_table *t)
{	
	t->philos = malloc(sizeof(t_philo) * t->nb_philo);
	if (!t->philos)
		return (destroy_table(t, t->nb_philo, 0), FALSE);
	t->i = 0;
	t->cleanup++;
	while (t->i < t->nb_philo)
	{
		t->philos[t->i].id = t->i + 1;
		t->philos[t->i].meals_eaten = 0;
		t->philos[t->i].last_meal_time = 0;
		t->philos[t->i].left_fork = &t->forks[t->i];
		t->philos[t->i].right_fork = &t->forks[(t->i + 1) % t->nb_philo];
		t->philos[t->i].table = t;
		memset(&t->philos[t->i].eat_lock, 0, sizeof(pthread_mutex_t));
		memset(&t->philos[t->i].meal_lock, 0, sizeof(pthread_mutex_t));
		if (pthread_mutex_init(&t->philos[t->i].meal_lock, NULL))
			return (destroy_table(t, t->nb_philo, t->i));
		if (pthread_mutex_init(&t->philos[t->i].eat_lock, NULL))
			return (destroy_table(t, t->nb_philo, t->i));
		if (pthread_mutex_init(&t->meal_check, NULL))
			return (destroy_table(t, t->nb_philo, t->i));
		// adjust cleanup var for this new mutex_init
		t->i++;
	}
	return (TRUE);
}

int	simulation_init(t_table *table, int ac, char **av)
{
	if (init_table(table, ac, av) == FALSE)
		return (FALSE);
	if (init_philos(table) == FALSE)
		return (FALSE);
	table->cleanup++;
	return (TRUE);
}
