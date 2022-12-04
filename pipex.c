/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra < abouabra@student.1337.ma >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 21:04:18 by abouabra          #+#    #+#             */
/*   Updated: 2022/12/04 16:27:06 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>




char *check_command_path(char *command, char **ev)
{
    char *path;
    char **tmp;
    char *str2;
    int i;
    int ret;

    ret = access(command, F_OK);
    if(!ret && ft_strrchr(command, '/') && ft_strrchr(command, '.'))
    {
        return command;
    }
    i = -1;
    path = NULL;
    while(ev[++i])
        if(!ft_strncmp(ev[i], "PATH=", 5))
            path = ev[i];
    if(!path)
        path = "PATH=/usr/local/bin:/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin";
    path = path + 5;
    tmp = ft_split(path, ':');
    i = -1;
    while(tmp[++i])
    {
        char *stt = ft_strrchr(command, '/');
        if(!stt)
            stt = command;
        else
        {
            stt++;
            command = stt;
        }
        char *str = ft_strjoin(tmp[i], "/");
        str2 = ft_strjoin(str, command);  
            
        ret = access(str2, F_OK);
        if(!ret)
        {
            // // printf("%s\n", str2);
            // while(tmp[i])
            //     free(tmp[i++]);
            // free(tmp);
            // tmp = NULL;
            return str2;
        }
        free(str2);
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
            // wa freeyini
            return 0;
        }
    }
    return fd;
}
void check_command(t_vars *vars)
{
    if(!vars->command_path)
        dprintf(2, "pipex: %s: command not found\n",* vars->exec_param);
    if(vars->command_path && access(vars->command_path, X_OK) == -1)
        dprintf(2, "pipex: permission denied: %s\n",* vars->exec_param);
}

int main(int ac,char **av,char **ev)
{
    if(ac <= 4)
        return 1;
    
    t_vars *vars;
    vars = ft_calloc(1, sizeof(t_vars));
    if(!vars)
        return 0;
    vars->fd = alloc_fd(ac-3);
    vars->ev = ev;
    vars->av = av;
    
    if(pipe(vars->fd[0]) == -1)
        return 1;
    int id = fork();
    if(id == -1)
        return 1;
    int in_fd=0;
    if(id == 0)
    {
        close(vars->fd[0][0]);
        in_fd = open(av[1],O_RDONLY | O_CREAT , 0644);
        if(in_fd == -1)
            return 1;
        dup2(in_fd, 0);
        dup2(vars->fd[0][1], 1);

        vars->exec_param = ft_split(av[2], ' ');
        vars->command_path = check_command_path(* vars->exec_param, ev);
        check_command(vars);
        
        execve(vars->command_path,vars->exec_param,vars->ev);
        exit(127);
    }
    
    int i = -1;
    while(++i < ac-5)
    {
        vars->exec_param = ft_split(vars->av[i + 3], ' ');
        vars->command_path = check_command_path(*vars->exec_param, vars->ev);
        check_command(vars);
        execute_command(vars,i);
    }
    
    vars->exec_param = ft_split(av[ac-2], ' ');
    vars->command_path = check_command_path(* vars->exec_param, ev);
    check_command(vars);
    
    int out_fd = open(av[ac-1],O_WRONLY | O_TRUNC | O_CREAT , 0644);
    if(out_fd == -1)
        return (1);
        

    dup2(vars->fd[i][0], 0);
    dup2(out_fd, 1);
    close(vars->fd[i][0]);
    close(vars->fd[i][1]);
    
    execve(vars->command_path, vars->exec_param,ev);
    
    if(vars->command_path && access(vars->command_path, X_OK) == -1)
        return 126;
    return 127;
}