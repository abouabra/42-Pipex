/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra < abouabra@student.1337.ma >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 21:04:18 by abouabra          #+#    #+#             */
/*   Updated: 2022/12/05 18:28:11 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char *check_command_path(char *command, char **ev)
{
    char *path;
    char **tmp;
    char *str2;
    int i;
    int ret;

    ret = access(command, F_OK);
    if(!ret && ft_strrchr(command, '/') && ft_strrchr(command, '.'))
        return command;
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
            return str2;
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
        ft_dprintf(2, "pipex: %s: command not found\n",* vars->exec_param);
    if(vars->command_path && access(vars->command_path, X_OK) == -1)
        ft_dprintf(2, "pipex: permission denied: %s\n",* vars->exec_param);
}

int main(int ac,char **av,char **ev)
{
    if(ac <= 4)
        return 1;
    
    t_vars *vars;
    vars = ft_calloc(1, sizeof(t_vars));
    
    if(!ft_strncmp(av[1], "here_doc", 8))
    {
        vars->is_herdoc = 1;
        if(!ft_strncmp(av[2], "", 1))
            vars->herdoc_limiter = "\n";
        else
            vars->herdoc_limiter = av[2];
    }

    //---------------
    int alloc_fd_size;
    if(vars->is_herdoc)
        alloc_fd_size = ac -4;
    else
        alloc_fd_size = ac -3;
    //---------------
    if(!vars)
        return 0;
    vars->fd = alloc_fd(alloc_fd_size);
    vars->ev = ev;
    vars->av = av;
    if(pipe(vars->fd[0]) == -1)
        return 1;
    int id = fork();
    if(id == -1)
        return 1;
    int in_fd=0;
    int command_index;
    if(id == 0)
    {
        close(vars->fd[0][0]);
        if(vars->is_herdoc)
        {
            char *tmp;
            char *total;
            total = "";
            while(1)
            {
                tmp = get_next_line(0);
                unsigned int len;
                if(ft_strlen(tmp) < ft_strlen(vars->herdoc_limiter) )
                    len = -1;
                else
                    len = (unsigned int) ft_strlen(tmp)-1;
                if(!ft_strncmp(tmp, vars->herdoc_limiter, len))
                    break;
                total = ft_strjoin(total, tmp);
            }
            in_fd = open("./tmp_herdoc",O_RDWR | O_TRUNC | O_CREAT , 0644);
            if(in_fd == -1)
                return 1;
            write(in_fd, total, ft_strlen(total));
            close(in_fd);
            in_fd = open("./tmp_herdoc",O_RDONLY | O_CREAT , 0644);
            if(in_fd == -1)
                return 1;
            dup2(in_fd, 0);
            dup2(vars->fd[0][1], 1);
            command_index = 3;
        }
        else
        {
            in_fd = open(av[1],O_RDONLY | O_CREAT , 0644);
            if(in_fd == -1)
                return 1;
            dup2(in_fd, 0);
            dup2(vars->fd[0][1], 1);
            command_index = 2;
        }
        unlink("./tmp_herdoc");
        vars->exec_param = ft_split(av[command_index], ' ');
        vars->command_path = check_command_path(* vars->exec_param, ev);
        check_command(vars);
        
        execve(vars->command_path,vars->exec_param,vars->ev);
        exit(127);
    }
    //-----------
    int while_index;

    if(vars->is_herdoc)
    {
        command_index = 4;
        while_index = ac-6;
    }
    else
    {
        command_index = 3;
        while_index = ac -5;
    }
    //-----------
    int i = -1;
    while(++i < while_index)
    {
        //ft_dprintf(2, "INDEX: %d || COMMAND: %s\n",while_index,vars->av[i+command_index]);
        vars->exec_param = ft_split(vars->av[i + command_index], ' ');
        vars->command_path = check_command_path(*vars->exec_param, vars->ev);
        check_command(vars);
        execute_command(vars,i);
    }
    
    vars->exec_param = ft_split(av[ac-2], ' ');
    vars->command_path = check_command_path(* vars->exec_param, ev);
    check_command(vars);
    int out_fd;
    
    if(vars->is_herdoc)
        out_fd = open(av[ac-1],O_RDWR | O_APPEND | O_CREAT , 0644);
    else   
        out_fd = open(av[ac-1],O_WRONLY | O_TRUNC | O_CREAT , 0644);
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