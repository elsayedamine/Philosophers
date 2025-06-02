/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:00:40 by aelsayed          #+#    #+#             */
/*   Updated: 2025/06/02 17:35:56 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_table	table;

	if (ac != 6 && ac != 5)
		return (write(2, "Usage:\n\t./philo number_of_philos time_to_die \
time_to_eat time_to_sleep [meals_required]\n", 89), EXIT_FAILURE);
	if (simulation_init(&table, ac, av) == FALSE)
		return (EXIT_FAILURE);
	// pthread_join(table.monitor, NULL);
	destroy_table(&table, table.nb_philo, table.nb_philo);
	return (EXIT_SUCCESS);
}

// void precise_sleep(long ms)
// {
	// long start = get_time();
	// while (get_time() - start < ms)
		// usleep(100); // small sleep to reduce CPU use
// }
