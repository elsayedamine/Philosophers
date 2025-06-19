/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:00:40 by aelsayed          #+#    #+#             */
/*   Updated: 2025/06/19 18:56:05 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

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
		printf("%ld %d %s\n", get_time() - \
		philo->table->start_time, philo->id, msg);
	pthread_mutex_unlock(&philo->table->death_lock);
	pthread_mutex_unlock(&philo->table->print_lock);
}

void	one_philo(t_table *t)
{
	t->start_time = get_time();
	pthread_mutex_lock(t->forks);
	print_state(t->philos, FORK);
	pthread_mutex_unlock(t->forks);
	usleep(t->time_to_die * 1000);
	printf("%ld %d died\n", get_time() - t->start_time, t->philos[0].id);
	destroy_table(t, t->nb_philo, t->nb_philo);
}

int	main(int ac, char **av)
{
	t_table	table;

	if (ac != 6 && ac != 5)
		return (write(2, ARG_ERR, 89), EXIT_FAILURE);
	if (init_table(&table, ac, av) == FALSE)
		return (EXIT_FAILURE);
	if (init_philos(&table) == FALSE)
		return (EXIT_FAILURE);
	if (table.nb_philo == 1)
		one_philo(&table);
	else
		simulation_play(&table);
	return (EXIT_SUCCESS);
}
