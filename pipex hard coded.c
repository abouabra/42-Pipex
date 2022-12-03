/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra < abouabra@student.1337.ma >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 21:04:18 by abouabra          #+#    #+#             */
/*   Updated: 2022/12/03 16:33:51 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>



char *check_command_path(char *command, char **ev)
{
    char *path;
    char **tmp;
    char *str2;
    int i;
    int ret;

    // // printf("COMAMD: %s\n",command);
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
    // // //////printf("PATH: %s\n",path);
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

int main(int ac,char **av,char **ev)
{
    if(ac <= 4)
        return 1;
    
    char **in_exec_param;
    char *in_command_path;
    
    char **out_exec_param;
    char *out_command_path;
    
    int in_fd;
    int out_fd;

    in_exec_param = ft_split(av[2], ' ');
    in_command_path = check_command_path(*in_exec_param, ev);
    if(!in_command_path)
    {
            
        char *str;
        char *str2 = ft_strjoin(ft_strdup("pipex: "),*in_exec_param);
        str = ft_strjoin(str2,ft_strdup(": command not found\n"));
        ft_putstr_fd(str, 2);
    }


    
    out_exec_param = ft_split(av[3], ' ');
    out_command_path = check_command_path(*out_exec_param, ev);
    if(!out_command_path)
    {
        // // printf("HELLO");
        char *str;
        char *str2 = ft_strjoin(ft_strdup("pipex: "),*out_exec_param);
        str = ft_strjoin(str2,ft_strdup(": command not found\n"));
        ft_putstr_fd(str, 2);
    }
    if(out_command_path && access(out_command_path, X_OK) == -1)
    {
        char *str;
        str = ft_strjoin(ft_strdup("pipex: "),"permission denied: ");
        str = ft_strjoin(str,*out_exec_param);
        str = ft_strjoin(str,"\n");
        ft_putstr_fd(str, 2);
    }

    int fd[2];
    int fd2[2];
    if(pipe(fd) == -1)
        return 1;
    int id = fork();
    if(id == -1)
        return 1;
    if(id == 0)
    {
        close(fd[0]);
        in_fd = open(av[1],O_RDONLY | O_CREAT , 0644);
        if(in_fd == -1)
            return 1;
        dup2(in_fd, 0);
        dup2(fd[1], 1);
        close(in_fd);
        execve(in_command_path,in_exec_param,ev);
        exit(127);
    }

    if(pipe(fd2) == -1)
        return 1;
    int id2 = fork();
    if(id2 == -1)
        return 1;
    if(id2 == 0)
    {
        close(fd[1]);
        close(fd2[0]);
        
        dup2(fd[0], 0);
        dup2(fd2[1], 1);

        in_exec_param = ft_split(av[3], ' ');
        in_command_path = check_command_path(*in_exec_param, ev);


        execve(in_command_path,in_exec_param,ev);
        exit(127);
    }
    
    close(fd[0]);
    close(fd[1]);

    id = fork();
    if(id == 0)
    {
        close(fd2[1]);
        out_fd = open(av[5],O_WRONLY | O_TRUNC | O_CREAT , 0644);
        if(out_fd == -1)
            return (1);

        dup2(fd2[0], 0);
        dup2(out_fd, 1);
        in_exec_param = ft_split(av[4], ' ');
        in_command_path = check_command_path(*in_exec_param, ev);

        close(out_fd);
        execve(in_command_path,in_exec_param,ev);
    }
    close(fd2[0]);
    close(fd2[1]);
    
    if(in_command_path && access(in_command_path, X_OK) == -1)
        return 126;
    return 127;
}