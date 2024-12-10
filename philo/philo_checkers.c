/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_checkers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damoncad <damoncad@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-12-04 19:24:29 by damoncad          #+#    #+#             */
/*   Updated: 2024-12-04 19:24:29 by damoncad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_done(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->done_mutex);
	if (data->done)
	{
		pthread_mutex_unlock(&data->done_mutex);
		return (1);
	}
	if (data->must_eat != -1 && philo->meals_eaten >= data->must_eat)
	{
		pthread_mutex_unlock(&data->done_mutex);
		if (are_all_meals_completed(data))
		{
			pthread_mutex_lock(&data->done_mutex);
			data->done = 1;
			pthread_mutex_unlock(&data->done_mutex);
			return (1);
		}
		return (0);
	}
	pthread_mutex_unlock(&data->done_mutex);
	return (0);
}

int	check_death(t_data *data, t_philo *philo)
{
	int			is_dead;
	long long	current;

	is_dead = 0;
	current = get_time();
	pthread_mutex_lock(&data->meal_mutex);
	if (current - data->last_meal_times[philo->id - 1] > data->time_to_die)
	{
		pthread_mutex_unlock(&data->meal_mutex);
		pthread_mutex_lock(&data->done_mutex);
		if (!data->done)
		{
			data->done = 1;
			pthread_mutex_unlock(&data->done_mutex);
			pthread_mutex_lock(&data->write_mutex);
			printf("%lld %d died\n", current - data->start_time, philo->id);
			pthread_mutex_unlock(&data->write_mutex);
			return (1);
		}
		pthread_mutex_unlock(&data->done_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->meal_mutex);
	return (0);
}

int	are_all_done(t_data *data)
{
	int	i;

	if (data->must_eat == -1)
		return (0);
	pthread_mutex_lock(&data->meal_mutex);
	i = 0;
	while (i < data->num_philos)
	{
		if (data->philos[i].meals_eaten < data->must_eat)
		{
			pthread_mutex_unlock(&data->meal_mutex);
			return (0);
		}
		i++;
	}
	pthread_mutex_unlock(&data->meal_mutex);
	return (1);
}

int	are_all_meals_completed(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->meal_mutex);
	i = 0;
	while (i < data->num_philos)
	{
		if (data->philos[i].meals_eaten < data->must_eat)
		{
			pthread_mutex_unlock(&data->meal_mutex);
			return (0);
		}
		i++;
	}
	pthread_mutex_unlock(&data->meal_mutex);
	return (1);
}

int	try(t_data *data, t_philo *philo)
{
	int	i;

	(void)philo;
	i = 0;
	while (i < data->num_philos)
	{
		if (check_death(data, &data->philos[i]) == 1)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}
