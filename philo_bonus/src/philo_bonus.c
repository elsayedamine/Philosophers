/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 23:55:10 by sayed             #+#    #+#             */
/*   Updated: 2025/06/22 22:36:13 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

void	close_sem(t_table *t)
{
	sem_close(t->forks);
	sem_unlink("/forks");
	sem_close(t->print);
	sem_unlink("/print");
	sem_close(t->death);
	sem_unlink("/death");
	return ;
}

int	main(int ac, char **av)
{
	t_table	table;

	if (ac != 6 && ac != 5)
		return (write(2, ARG_ERR, 100), EXIT_FAILURE);
	init_table(&table, ac, av);
	init_philos(&table);
	if (table.nb_philo == 1)
		one_philo(&table);
	else
		simulation(&table);
	return (EXIT_SUCCESS);
}
