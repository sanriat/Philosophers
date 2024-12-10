/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: damoncad <damoncad@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024-11-14 17:26:42 by damoncad          #+#    #+#             */
/*   Updated: 2024-11-14 17:26:42 by damoncad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				meals_eaten;
	long long		last_meal_time;
	struct s_data	*data;
	pthread_t		thread;
}	t_philo;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				done;
	long long		start_time;
	long long		*last_meal_times;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	done_mutex;
}	t_data;

//philo.c
void		clean(t_data data);
int			init_philos(t_data *data);
int			init_data(t_data *data, int ac, char **av);
int			init_mutexes(t_data *data);

//philo_help.c
void		numbs(t_data *data, char **av, int ac);
void		*philo(void *arg);
int			finally_done(t_data *data);

//philo_checkers.c
int			philo_done(t_philo *philo, t_data *data);
int			check_death(t_data *data, t_philo *philo);
int			are_all_done(t_data *data);
int			are_all_meals_completed(t_data *data);
int			try(t_data *data, t_philo *philo);

//philo_actions.c
int			death(t_data *data, t_philo *philo, int is_dead, long long current);
void		eat(t_philo *philo);
void		eating(t_philo *philo, t_data *data,
				int first_fork, int second_fork);
void		ft_sleep(t_philo *philo);
void		think(t_philo *philo, int in);

//philo_support.c
int			ft_atoi(char *str);
long long	get_time(void);
void		print_status(t_data *data, int id, char *status);
int			ips(char *str);
void		smart_sleep(long long time, t_data *data, t_philo *philo);

#endif