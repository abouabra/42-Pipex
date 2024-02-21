/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra < abouabra@student.1337.ma >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 21:04:18 by abouabra          #+#    #+#             */
/*   Updated: 2022/12/07 16:47:33 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**get_path(char **ev)
{
	int		i;
	char	*path;

	i = -1;
	path = NULL;
	while (ev[++i])
		if (ft_strnstr(ev[i], "PATH=", 5))
			path = ev[i] + 5;
	if (!path)
		path = "/usr/local/bin:/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin";
	return (ft_split(path, ':'));
}

void	free_path(char **path)
{
	int	i;

	i = -1;
	while (path[++i])
		free(path[i]);
	free(path);
}

char	*get_command_path(char **path, char *command)
{
	int		i;
	int		ret;
	char	*str;
	char	*stt;

	i = -1;
	while (path[++i])
	{
		stt = ft_strrchr(command, '/');
		if (!stt)
			stt = command;
		else
			command = ++stt;
		str = ft_strjoin_gnl(ft_strjoin(path[i], "/"), command);
		ret = access(str, F_OK);
		if (!ret)
		{
			free_path(path);
			return (str);
		}
		free(str);
	}
	return (NULL);
}

char	*check_command_path(char *command, char **ev)
{
	char	**path;
	int		ret;
	char	*str;

	ret = access(command, F_OK);
	if (!ret && ft_strrchr(command, '/') && ft_strrchr(command, '.'))
		return (command);
	path = get_path(ev);
	str = get_command_path(path, command);
	return (str);
}

void	execute_command(t_vars *vars, int i)
{
	int	id;

	if (pipe(vars->fd[i + 1]) == -1)
		return ;
	id = fork();
	if (id == -1)
		return ;
	if (id == 0)
	{
		close(vars->fd[i][1]);
		close(vars->fd[i + 1][0]);
		dup2(vars->fd[i + 1][1], 1);
		dup2(vars->fd[i][0], 0);
		execve(vars->command_path, vars->exec_param, vars->ev);
		exit(127);
	}
	close(vars->fd[i][0]);
	close(vars->fd[i][1]);
}

void	free_fd(int size, int **fd)
{
	int	i;

	i = -1;
	while (++i < size)
		free(fd[i]);
	free(fd);
}

int	**alloc_fd(int size)
{
	int	**fd;
	int	i;

	fd = malloc((size) * sizeof(int *));
	if (!fd)
		return (0);
	i = -1;
	while (++i < size)
	{
		fd[i] = malloc(2 * sizeof(int));
		if (!fd[i])
		{
			free_fd(size, fd);
			return (0);
		}
	}
	return (fd);
}

void	check_command(t_vars *vars)
{
	if (!vars->command_path)
		ft_dprintf(2, "pipex: %s: command not found\n", *vars->exec_param);
	if (vars->command_path && access(vars->command_path, X_OK) == -1)
		ft_dprintf(2, "pipex: permission denied: %s\n", *vars->exec_param);
}

void	set_open_stuff2(t_vars *vars)
{
	if (vars->is_herdoc)
		vars->out_fd = open(vars->av[vars->ac - 1],
				O_RDWR | O_APPEND | O_CREAT, 0644);
	else
		vars->out_fd = open(vars->av[vars->ac - 1],
				O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (vars->out_fd == -1)
		exit(1);
}

void	set_open_stuff1(t_vars *vars)
{
	if (vars->is_herdoc)
		vars->in_fd = open("./tmp_herdoc", O_RDONLY | O_CREAT, 0644);
	else
		vars->in_fd = open(vars->av[1], O_RDONLY);
	if (vars->in_fd == -1)
	{
		if (access(vars->av[1], F_OK) == -1)
			ft_dprintf(2, "pipex: %s: No such file or directory\n",
				vars->av[1]);
		else if (access(vars->av[1], R_OK) == -1)
			ft_dprintf(2, "pipex: permission denied: %s\n", vars->av[1]);
		exit(1);
	}
}

void	set_parameters(t_vars *vars, int ac, char **av, char **ev)
{
	if (vars->is_herdoc)
	{
		vars->alloc_fd_size = ac - 4;
		vars->command_index = 3;
		vars->while_command = 4;
		vars->while_index = ac - 6;
	}
	else
	{
		vars->alloc_fd_size = ac - 3;
		vars->command_index = 2;
		vars->while_command = 3;
		vars->while_index = ac - 5;
	}
	vars->fd = alloc_fd(vars->alloc_fd_size);
	vars->av = av;
	vars->ac = ac;
	vars->ev = ev;
}

char	*heredoc_text(t_vars *vars)
{
	char			*tmp;
	char			*total;
	unsigned int	len;

	total = "";
	while (1)
	{
		tmp = get_next_line(0);
		if (ft_strlen(tmp) < ft_strlen(vars->herdoc_limiter))
			len = -1;
		else
			len = (unsigned int)ft_strlen(tmp) - 1;
		if (!ft_strncmp(tmp, vars->herdoc_limiter, len))
			break ;
		total = ft_strjoin(total, tmp);
	}
	return (total);
}

void	get_herdoc_data(t_vars *vars)
{
	char	*total;
	int		tmp_fd;

	if (vars->is_herdoc)
	{
		total = heredoc_text(vars);
		tmp_fd = open("./tmp_herdoc", O_RDWR | O_TRUNC | O_CREAT, 0644);
		if (tmp_fd == -1)
			exit(1);
		write(tmp_fd, total, ft_strlen(total));
		close(tmp_fd);
	}
}

void	first_command(t_vars *vars)
{
	int	id;

	if (pipe(vars->fd[0]) == -1)
		exit(1);
	id = fork();
	if (id == -1)
		exit(1);
	if (id == 0)
	{
		close(vars->fd[0][0]);
		get_herdoc_data(vars);
		set_open_stuff1(vars);
		dup2(vars->in_fd, 0);
		dup2(vars->fd[0][1], 1);
		unlink("./tmp_herdoc");
		vars->exec_param = ft_split(vars->av[vars->command_index], ' ');
		vars->command_path = check_command_path(*vars->exec_param, vars->ev);
		check_command(vars);
		execve(vars->command_path, vars->exec_param, vars->ev);
		exit(127);
	}
}

int	last_command(t_vars *vars, int i)
{
	set_open_stuff2(vars);
	vars->exec_param = ft_split(vars->av[vars->ac - 2], ' ');
	vars->command_path = check_command_path(*vars->exec_param, vars->ev);
	check_command(vars);
	dup2(vars->fd[i][0], 0);
	dup2(vars->out_fd, 1);
	close(vars->fd[i][0]);
	close(vars->fd[i][1]);
	execve(vars->command_path, vars->exec_param, vars->ev);
	if (vars->command_path && access(vars->command_path, X_OK) == -1)
		return (126);
	return (127);
}

void	n_commands(t_vars *vars)
{
	vars->i = -1;
	while (++(vars->i) < vars->while_index)
	{
		vars->exec_param = ft_split(vars->av[vars->i
				+ vars->while_command], ' ');
		vars->command_path = check_command_path(*vars->exec_param, vars->ev);
		check_command(vars);
		execute_command(vars, vars->i);
	}
}

int	main(int ac, char **av, char **ev)
{
	t_vars	*vars;
	int		return_val;

	if (ac <= 4)
	{
		ft_dprintf(1,"Usage: ./pipex InFile cmd1 cmd2 ... cmdN OutFile\n");
		return (1);
	}
	vars = ft_calloc(1, sizeof(t_vars));
	if (!vars)
		return (1);
	if (!ft_strncmp(av[1], "here_doc", 8))
	{
		vars->is_herdoc = 1;
		if (!ft_strncmp(av[2], "", 1))
			vars->herdoc_limiter = "\n";
		else
			vars->herdoc_limiter = av[2];
	}
	set_parameters(vars, ac, av, ev);
	first_command(vars);
	n_commands(vars);
	return_val = last_command(vars, vars->i);
	return (return_val);
}
