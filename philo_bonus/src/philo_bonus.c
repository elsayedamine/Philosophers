/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sayed <sayed@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:55:10 by sayed             #+#    #+#             */
/*   Updated: 2025/06/17 10:00:16 by sayed            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int	main(int ac, char **av)
{
	t_table	table;

	if (ac != 6 && ac != 5)
		return (write(2, ARG_ERR, 89), EXIT_FAILURE);
	init_table(&table, ac, av);
	init_philos(&table);
	simulation(&table);
	// if (table.nb_philo == 1)
	// 	one_philo(&table);
	// else
	// 	simulation_play(&table);
	return (EXIT_SUCCESS);
}
