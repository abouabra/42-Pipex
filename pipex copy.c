/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra < abouabra@student.1337.ma >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 21:04:18 by abouabra          #+#    #+#             */
/*   Updated: 2022/12/05 20:44:30 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

char **get_path(char **ev)
{
    char *tmp;
    int i;
    
    tmp = NULL;
    i=-1;
    while(ev[++i])
        if(ft_strnstr(ev[i],"PATH=",-1))
            tmp = ev[i] +5;
    if(!tmp)
        tmp = "/usr/local/bin:/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin";
    return ft_split(tmp, ':');
}

char *check_command_path(char *command, char **ev)
{
    char **path;
    char *str2;
    int i;
    int ret;

    ret = access(command, F_OK);
    if(!ret && ft_strrchr(command, '/') && ft_strrchr(command, '.'))
        return command;
    path = get_path(ev);
    i = -1;
    while(path[++i])
    {
        char *stt = ft_strrchr(command, '/');
        if(!stt)
            stt = command;
        else
            command = ++stt;
        str2 = ft_strjoin(ft_strjoin(path[i], "/"), command);
        if(!access(str2, F_OK))
            return str2;
        //free(str2);
    }
    return NULL;
}

void execute_command(t_vars *vars, int i)
{
    if(pipe(vars->fd[i + 1]) == -1)
        return;

    int id = fork();
    if(id == -1)
        return ;
    if(id == 0)
    {
        close(vars->fd[i][1]);
        close(vars->fd[i + 1][0]);
        
        dup2(vars->fd[i + 1][1], 1);
        dup2(vars->fd[i][0], 0);
        
        execve(vars->command_path,vars->exec_param,vars->ev); 
        exit(127);
    }
    close(vars->fd[i][0]);
    close(vars->fd[i][1]);
}

void    free_fd(int size, int **fd)
{
    int i;

    i= -1;
    while(++i < size)
        free(fd[i]);
    free(fd);
}

int **alloc_fd(int size)
{
    int **fd;
    fd = malloc((size) * sizeof(int *));
    if(!fd)
        return 0;
    int i =-1;
    while(++i < size)
    {
        fd[i] = malloc(2 * sizeof(int));
        if(!fd[i])
        {
            free_fd(size,fd);
            return 0;
        }
    }
    return fd;
}

char *heredoc_text(t_vars *vars)
{
    char *tmp;
    char *total;
    total = "";
    while(1)
    {
        tmp = get_next_line(0);
        unsigned int len;
        if(ft_strlen(tmp) < ft_strlen(vars->herdoc_limiter))
            len = -1;
        else
            len = (unsigned int) ft_strlen(tmp)-1;
        if(!ft_strncmp(tmp, vars->herdoc_limiter, len))
            break;
        total = ft_strjoin(total, tmp);
    }
    return total;
}

void check_command(t_vars *vars)
{
    if(!vars->command_path)
        ft_dprintf(2, "pipex: %s: command not found\n",* vars->exec_param);
    if(vars->command_path && access(vars->command_path, X_OK) == -1)
        ft_dprintf(2, "pipex: permission denied: %s\n",* vars->exec_param);
}

void    set_parameters(t_vars *vars)
{
    if(vars->is_herdoc)
    {
        vars->alloc_fd_size = vars->ac -4;
        vars->command_index = 3;
        vars->while_command = 4;
        vars->while_index = vars->ac-6;
    }
    else
    {
        vars->alloc_fd_size = vars->ac -3;
        vars->command_index = 2;
        vars->while_command = 3;
        vars->while_index = vars->ac -5;
    }
}

void    set_open_stuff(t_vars *vars)
{
        if(vars->is_herdoc)
        {
            char *total = heredoc_text(vars);
            int tmp_fd = open("./tmp_herdoc",O_RDWR | O_TRUNC | O_CREAT , 0644);
            if(tmp_fd == -1)
                exit(1);
            write(tmp_fd, total, ft_strlen(total));
            close(tmp_fd);
            vars->in_fd = open("./tmp_herdoc",O_RDONLY | O_CREAT , 0644);
            vars->out_fd = open(vars->av[vars->ac-1],O_RDWR | O_APPEND | O_CREAT , 0644);
        }
        else   
        {
            vars->in_fd = open(vars->av[1],O_RDONLY | O_CREAT , 0644);
            vars->out_fd = open(vars->av[vars->ac-1],O_WRONLY | O_TRUNC | O_CREAT , 0644);
        }
        if(vars->out_fd == -1 || vars->in_fd == -1)
            exit(1);       

}

int main(int ac,char **av,char **ev)
{
    if(ac <= 4)
        return 1;
    
    t_vars *vars;
    vars = ft_calloc(1, sizeof(t_vars));
    if(!vars)
        return 0;
    
    if(!ft_strncmp(av[1], "here_doc", 8))
    {
        vars->is_herdoc = 1;
        if(!ft_strncmp(av[2], "", 1))
            vars->herdoc_limiter = "\n";
        else
            vars->herdoc_limiter = av[2];
    }

    set_parameters(vars);

    vars->fd = alloc_fd(vars->alloc_fd_size);
    vars->av = av;
    vars->ac = ac;
    vars->ev = ev;
    
    if(pipe(vars->fd[0]) == -1)
        return 1;

    int id = fork();
    if(id == -1)
        return 1;
    if(id == 0)
    {
        close(vars->fd[0][0]);
        set_open_stuff(vars);
        
        dup2(vars->in_fd, 0);
        dup2(vars->fd[0][1], 1);
        unlink("./tmp_herdoc");
        vars->exec_param = ft_split(av[vars->command_index], ' ');
        vars->command_path = check_command_path(* vars->exec_param, ev);
        check_command(vars);
        
        execve(vars->command_path,vars->exec_param,vars->ev);
        exit(127);
    }
    
    int i = -1;
    while(++i < vars->while_index)
    {
        vars->exec_param = ft_split(vars->av[i + vars->while_command], ' ');
        vars->command_path = check_command_path(*vars->exec_param, vars->ev);
        check_command(vars);
        execute_command(vars,i);
    }
    
    vars->exec_param = ft_split(av[ac-2], ' ');
    vars->command_path = check_command_path(*vars->exec_param, ev);
    check_command(vars);
    
    dup2(vars->fd[i][0], 0);
    dup2(vars->out_fd, 1);
    close(vars->fd[i][0]);
    close(vars->fd[i][1]);
    execve(vars->command_path, vars->exec_param,ev);
    
    if(vars->command_path && access(vars->command_path, X_OK) == -1)
        return 126;
    return 127;
}