/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damoncad <damoncad@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-06 14:51:45 by damoncad          #+#    #+#             */
/*   Updated: 2024-11-06 14:51:45 by damoncad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	numbs(t_data *data, char **av, int ac)
{
	data->num_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->must_eat = ft_atoi(av[5]);
	else
		data->must_eat = -1;
}

void	*philo(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	think(philo, 1);
	if (philo->id % 2 == 0)
	{
		usleep(10);
		think(philo, 0);
	}
	while (1)
	{
		if (finally_done(data) || try(data, philo))
			break ;
		eat(philo);
		if (data->num_philos == 1)
			break ;
		ft_sleep(philo);
		think(philo, 0);
	}
	return (NULL);
}

int	finally_done(t_data *data)
{
	pthread_mutex_lock(&data->done_mutex);
	if (are_all_done(data))
	{
		data->done = 1;
		pthread_mutex_unlock(&data->done_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->done_mutex);
	return (0);
}
