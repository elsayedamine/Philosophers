/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 11:06:29 by aelsayed          #+#    #+#             */
/*   Updated: 2025/06/19 18:49:50 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// void	monitor(t_table *t)
// {
// 	int		status;
// 	int		i;
// 	pid_t	pid;
// 	// Wait for ANY child to exit
// 	pid = waitpid(-1, &status, 0);
// 	if (pid > 0 && WIFEXITED(status) && WEXITSTATUS(status) == 1)
// 	{
// 		// A philosopher died - kill all others immediately
// 		i = 0;
// 		while (i < t->nb_philo)
// 		{
// 			if (t->philos[i].pid != pid && t->philos[i].pid != 0)
// 				kill(t->philos[i].pid, SIGKILL);
// 			i++;
// 		}
// 	}
// 	// Clean up all remaining processes
// 	i = 0;
// 	while (i < t->nb_philo)
// 	{
// 		if (t->philos[i].pid != pid)
// 			waitpid(t->philos[i].pid, NULL, 0);
// 		i++;
// 	}
// 	sem_close(t->forks);
// 	sem_close(t->print);
// 	sem_close(t->death);
// 	cleanup(t, 0);
// }
