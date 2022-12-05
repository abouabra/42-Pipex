/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra < abouabra@student.1337.ma >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 21:04:28 by abouabra          #+#    #+#             */
/*   Updated: 2022/12/04 18:35:05 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
#define PIPEX_H

#include "libft/libft.h"
#include <fcntl.h>

typedef struct s_vars{
    char **exec_param;
    char *command_path;
    int **fd;
    char **av;
    char **ev;
    int     is_herdoc;
    char    *herdoc_limiter;
} t_vars;

#endif