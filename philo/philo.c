/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damoncad <damoncad@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-06 14:37:12 by damoncad          #+#    #+#             */
/*   Updated: 2024-11-06 14:37:12 by damoncad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean(t_data data)
{
	int	i;

	i = 0;
	while (i < data.num_philos)
		pthread_mutex_destroy(&data.forks[i++]);
	free(data.philos);
	free(data.forks);
	free(data.last_meal_times);
}

int	init_philos(t_data *data)
{
	int	i;

	i = 0;
	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (printf("Error: failed to allocate memory[philos]\n"));
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].left_fork = i;
		data->philos[i].right_fork = (i + 1) % data->num_philos;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = data->start_time;
		data->philos[i].data = data;
		i++;
	}
	return (EXIT_SUCCESS);
}

int	init_data(t_data *data, int ac, char **av)
{
	int	i;

	i = -1;
	if (!ips(av[1]) || !ips(av[2]) || !ips(av[3]) || !ips(av[4])
		|| (ac == 6 && !ips(av[5])))
		return (printf("Error: arguments must be positive integers\n"));
	numbs(data, av, ac);
	data->done = 0;
	data->start_time = get_time();
	data->last_meal_times = malloc(sizeof(long long) * data->num_philos);
	if (!data->last_meal_times)
		clean(*data);
	while (++i < data->num_philos)
		data->last_meal_times[i] = data->start_time;
	if (data->num_philos <= 0 || data->time_to_die <= 0
		|| data->time_to_eat <= 0 || data->time_to_sleep <= 0
		|| (ac == 6 && data->must_eat <= 0))
	{
		clean(*data);
		return (printf("Error: invalid arguments\n"));
	}
	return (EXIT_SUCCESS);
}

int	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (printf("Error: failed to allocate memory[forks]\n"));
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (printf("Error: failed to initialize mutex\n"));
		i++;
	}
	if (pthread_mutex_init(&data->write_mutex, NULL)
		|| pthread_mutex_init(&data->meal_mutex, NULL)
		|| pthread_mutex_init(&data->done_mutex, NULL))
		return (printf("Error: failed to initialize mutex\n"));
	return (EXIT_SUCCESS);
}

int	main(int ac, char **av)
{
	t_data	data;
	int		i;

	if (ac != 5 && ac != 6)
		return (printf("Error: wrong number of arguments\n"));
	if (init_data(&data, ac, av) || init_mutexes(&data)
		|| init_philos(&data))
		return (EXIT_FAILURE);
	i = -1;
	while (++i < data.num_philos)
	{
		if (pthread_create(&data.philos[i].thread, NULL, philo,
				&data.philos[i]))
			return (printf("Error: failed to create thread\n"));
	}
	i = -1;
	while (++i < data.num_philos)
	{
		if (pthread_join(data.philos[i].thread, NULL))
			return (printf("Error: failed to join thread\n"));
	}
	clean(data);
	return (EXIT_SUCCESS);
}
