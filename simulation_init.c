/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 21:01:16 by aelsayed          #+#    #+#             */
/*   Updated: 2025/06/01 22:30:36 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	destroy_table(t_table *table, int count, int philo_count)
{
	int i;

	i = 0;
	while (i < count)
		pthread_mutex_destroy(&table->forks[i++]);
	free(table->forks);
	if (table->cleanup >= 1)
		pthread_mutex_destroy(&table->print_lock);
	if (table->cleanup >= 2)
		pthread_mutex_destroy(&table->death_lock);
	if (table->cleanup == 3 && table->philos)
		free(table->philos);
	i = 0;
	if (table->cleanup >= 4)
		while (i < philo_count)
			pthread_mutex_destroy(&table->philos[i++].meal_lock);
	i = 0;
	if (table->cleanup >= 4)
		while (i < philo_count)
			pthread_mutex_destroy(&table->philos[i++].eat_lock);
	return (FALSE);
}

int	init_table(t_table *table, int ac, char **av)
{
	table->itr = 0;
	table->cleanup = 0;
	table->nb_philo = myatoi(av[1]);
	table->time_to_die = myatoi(av[2]);
	table->time_to_eat = myatoi(av[3]);
	table->time_to_sleep = myatoi(av[4]);
	table->meals_required = myatoi(av[5]);
	if (table->nb_philo <= 0 || table->time_to_die == -1 || \
		table->time_to_eat == -1 || table->time_to_sleep == -1 || \
			(ac == 6 && table->meals_required == -1))
		return (FALSE);
	table->start_time = 0;
	table->someone_died = FALSE;
	table->philos = NULL;
	table->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * table->nb_philo);
	if (!table->forks)
		return (FALSE);
	while (table->itr < table->nb_philo)
		if (pthread_mutex_init(&table->forks[table->itr++], NULL))
			return (destroy_table(table, table->itr - 1, 0));
	if (pthread_mutex_init(&table->print_lock, NULL))
		return (destroy_table(table, table->nb_philo, 0));
	if (pthread_mutex_init(&table->death_lock, NULL))
		return (table->cleanup++, destroy_table(table, table->nb_philo, 0));
	return (table->cleanup++, TRUE);
}

int	init_philos	(t_table *table)
{	
	table->philos = malloc(sizeof(t_philo) * table->nb_philo);
	if (!table->philos)
		return (destroy_table(table, table->nb_philo, 0), FALSE);
	table->itr = 0;
	table->cleanup++;
	while (table->itr < table->nb_philo)
	{
		table->philos[table->itr].id = table->itr + 1;
		table->philos[table->itr].meals_eaten = 0;
		table->philos[table->itr].last_meal_time = 0;
		table->philos[table->itr].left_fork = &table->forks[table->itr];
		table->philos[table->itr].right_fork = &table->forks[(table->itr + 1) % table->nb_philo];
		table->philos[table->itr].table = table;
		if (pthread_mutex_init(&table->philos[table->itr].meal_lock, NULL))
			return (destroy_table(table, table->nb_philo, table->itr));
		if (pthread_mutex_init(&table->philos[table->itr].eat_lock, NULL))
			return (destroy_table(table, table->nb_philo, table->itr));
		table->itr++;
	}
	return (TRUE);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	init_simulation_data(t_table *table, int ac, char **av)
{
	if (init_table(table, ac, av))
		return (FALSE);
	if (init_philos(table))
		return (FALSE);
	if (create_threads(table))
		return (FALSE);
	return (TRUE);
}

void	*routine(void)
{
	// does nothing now 
	// but it will use to run the routine of each philo
}

int	create_threads(t_table *table)
{
	int	i;

	table->start_time = get_time();
	i = 0;
	while (i < table->nb_philo)
	{
		table->philos[i].last_meal_time = table->start_time;
		if (pthread_create(&table->philos[i].thread_id, NULL, routine, &table->philos[i++]))
			return (table->someone_died = TRUE, destroy_table(table, \
				table->nb_philo, table->nb_philo), FALSE);
	}
		// idk if i ma going to use a monitor thread or not
		// but this is where i will create its thread
		// the  is hould while on the philos (threads) and detach them ?
	return (TRUE);	
}
