/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelsayed <aelsayed@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 09:59:40 by sayed             #+#    #+#             */
/*   Updated: 2025/06/18 01:38:13 by aelsayed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	kill_all_philosophers(t_table *t)
{
	int i = 0;
	while (i < t->nb_philo)
	{
		kill(t->philos[i].pid, SIGKILL);
		waitpid(t->philos[i].pid, NULL, 0);
		i++;
	}
}
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
// 		if (t->philos[i].pid != pid) // Don't wait for the one that already exited
// 			waitpid(t->philos[i].pid, NULL, 0);
// 		i++;
// 	}
// 	sem_close(t->forks);
// 	sem_close(t->print);
// 	sem_close(t->death);
// 	cleanup(t, 0);
// }

void	death_signal_handler(int sig)
{
	(void)sig;
	exit(0);
}
int	check_death(t_philo *p)
{
	if (get_time() - p->last_meal_time >= p->t->time_to_die)
	{
		sem_wait(p->t->print);
		printf("%ld %d died\n", get_time() - p->t->start_time, p->id);
		sem_post(p->t->print);
		exit(1);
	}
	return (TRUE);
}
// Modified routine to handle signals
void	routine(t_philo *p)
{
	signal(SIGTERM, death_signal_handler);
	signal(SIGKILL, death_signal_handler);
	
	while (TRUE)
	{
		check_death(p);  // Check #1: at start of loop
		
		sem_wait(p->t->forks);
		print_state(p, FORK);
		sem_wait(p->t->forks);
		print_state(p, FORK);
		
		if (process_eating(p) == FALSE)
			continue;
		if (process_sleeping(p) == FALSE)
			continue;
		if (process_thinking(p) == FALSE)
			continue;
		usleep(500);
	}
	exit(0);
}

// Improved monitor with faster killing
void	monitor(t_table *t)
{
	int		status;
	int		i;
	pid_t	pid;

	pid = waitpid(-1, &status, 0);
	
	if (pid > 0 && WIFEXITED(status) && WEXITSTATUS(status) == 1)
	{
		i = -1;
		while (++i < t->nb_philo)
			if (t->philos[i].pid > 0)
				kill(t->philos[i].pid, SIGKILL);
	}
	i = 0;
	while (i < t->nb_philo)
	{
		if (t->philos[i].pid > 0)
			waitpid(t->philos[i].pid, NULL, 0);
		i++;
	}
	cleanup(t, 0);
}

// void	routine(t_philo *p)
// {
// 	while (TRUE)
// 	{
// 		if (is_dead_or_full(p))
// 			break ;
// 		sem_wait(p->t->forks);
// 		print_state(p, FORK);
// 		sem_wait(p->t->forks);
// 		print_state(p, FORK);
// 		if (process_eating(p) == FALSE)
// 			continue ;
// 		if (process_sleeping(p) == FALSE)
// 			continue ;
// 		if (process_thinking(p) == FALSE)
// 			continue ;
// 		usleep(500);
// 	}
// 	exit(1);
// }

void	check_death_and_full(t_philo *p, long now, int full)
{
	(void)full;
	sem_wait(p->t->meal);
	if (now - p->last_meal_time >= p->t->time_to_die)
	{
		sem_post(p->t->meal);
		sem_wait(p->t->death);
		p->t->someone_died = TRUE;
		sem_post(p->t->death);
		sem_wait(p->t->print);
		printf("%ld %d died\n", get_time() - \
		p->t->start_time, p->id);
		sem_post(p->t->print);
		exit(1);
	}
	if (p->t->meals_required != -1 && p->meals_eaten >= p->t->meals_required)
	{
		sem_post(p->t->meal);
		exit(0);
	}
	sem_post(p->t->meal);
}

void	*death_monitor(void *philo)
{
	t_philo *p;

	p = (t_philo *)philo;
	while (TRUE)
	{
		sem_wait(p->t->death);
		if (p->t->someone_died || p->t->all_full == p->t->nb_philo)
		{
			sem_post(p->t->death);
			break ;
		}
		sem_post(p->t->death);
		check_death_and_full(p, get_time(), 0);
		usleep(500);
	}
	return (NULL);
}

int simulation(t_table *t)
{
	t->i = 0;
	t->start_time = get_time();
	while (t->i < t->nb_philo)
	{
		t->philos[t->i].pid = fork();
		if (t->philos[t->i].pid < 0)
			return (perror("fork"), cleanup(t, 0));
		if (t->philos[t->i].pid == 0)
		{
			t->philos[t->i].last_meal_time = get_time();
			// i removed the death_thread, it seems useless
			routine(&t->philos[t->i]);
		}
		t->i++;
	}
	monitor(t);
	return (TRUE);	
}
