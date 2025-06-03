/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:00:40 by aelsayed          #+#    #+#             */
/*   Updated: 2025/06/03 02:24:17 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*one_philo(t_table *t)
{
	pthread_mutex_lock(t->forks);
	print_state(t->philos, FORK);
	pthread_mutex_unlock(t->forks);
	usleep(t->time_to_die * 1000);
	printf("%ld %d died\n", get_time() - t->start_time, t->philos[0].id);
	return (NULL);
}
int	main(int ac, char **av)
{
	t_table	table;

	if (ac != 6 && ac != 5)
		return (write(2, ARG_ERR, 89), EXIT_FAILURE);
	///////////////////////////////////////////////////////////////
	if (simulation_init(&table, ac, av) == FALSE)
		return (EXIT_FAILURE);
	if (table.nb_philo == 1)
		one_philo(&table);
	else if (simulation_play(&table) == FALSE)
		return (EXIT_FAILURE);
	destroy_table(&table, table.nb_philo, table.nb_philo);
	return (EXIT_SUCCESS);
}

// void precise_sleep(long ms)
// {
	// long start = get_time();
	// while (get_time() - start < ms)
		// usleep(100); // small sleep to reduce CPU use
// }
