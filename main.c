/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:00:40 by aelsayed          #+#    #+#             */
/*   Updated: 2025/06/01 21:47:19 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_table	table;

	if (ac != 6 && ac != 5)
		return (write(2, "Usage:\n\t./philo number_of_philos time_to_die \
			time_to_eat time_to_sleep [meals_required]\n", 90), EXIT_FAILURE);
	if (init_simulation_data(&table, ac, av))
		return (EXIT_FAILURE);
	// start_simulation(&table);
	// destroy_simulation(&table);
	return (0);
}
