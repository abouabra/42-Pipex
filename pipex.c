/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabra < abouabra@student.1337.ma >      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 21:04:18 by abouabra          #+#    #+#             */
/*   Updated: 2022/11/26 19:35:17 by abouabra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "libft/libft.h"
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

char *check_command_path(char *command, char **ev)
{
    char *path;
    char **tmp;
    //char *str;
    char *str2;
    int i;
    int ret;
    
    
    i = -1;
    while(ev[++i])
        if(!ft_strncmp(ev[i], "PATH=", 5))
            path = ev[i];
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
        ////printf("---%s-%s--\n",str2,stt);
        // if(!ft_strncmp(stt, command, ft_strlen(command)))
        // {
        //     ////printf("---%s---\n",command);
        //     return command;
        // }   
            
        ret = access(str2, F_OK);
        if(!ret)
        {
            // //printf("%s\n", str2);
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
    /*
        RETURN ERRORS:
        ac <= 4             : 1
        fork                : 2
        open                : 3
        pipe                : 4
        command not found   : 127
    */
    char **in_exec_param;
    char *in_command_path;
    
    char **out_exec_param;
    char *out_command_path;
    
    int in_fd;
    int out_fd;



    
 
    //printf("IN Command: %s\n",av[2]);
    //printf("OUT Command: %s\n",av[3]);
    in_exec_param = ft_split(av[2], ' ');

    in_command_path = check_command_path(*in_exec_param, ev);
    if(!in_command_path)
    {
        char *str;
        str = ft_strjoin(*in_exec_param, ft_strdup(": command not found\n"));
        write(2, str, ft_strlen(str));
        //perror(str);
        //free(str);
        //return -1;
    }
        
        //perror(*in_exec_param);
    ////printf("\n");
    
    out_exec_param = ft_split(av[3], ' ');
    ////printf("AWK: |%s|\n",out_exec_param[1]);
    //int i=0;
    //while(out_exec_param[i])
    //    //printf("COMMAND: %s\n",out_exec_param[i++]);
    ////printf("\n\n");
    ////printf("STR: %s\n",*(out_exec_param +1));
    out_command_path = check_command_path(*out_exec_param, ev);
    if(!out_command_path)
    {
        char *str;
        str = ft_strjoin(*out_exec_param, ft_strdup(": command not found\n"));
        write(2, str, ft_strlen(str));
        //perror(str);
        //free(str);
        //return -1;
    }
        //perror(*out_exec_param);
    //printf("IN Command PATH: %s\n",in_command_path);
    //printf("OUT Command PATH: %s\n",out_command_path);
    //printf("\n");

    int fd[2];
    if(pipe(fd) == -1)
        return 4;
    
    //int status;
    int id = fork();
    if(id == -1)
        return 2;
    if(id == 0)
    {
        close(fd[0]);
        in_fd = open(av[1],O_RDONLY | O_CREAT , 0644);
        if(in_fd == -1)
            return 3;
        dup2(in_fd, 0);
        dup2(fd[1], 1);
        close(in_fd);
        execve(in_command_path,in_exec_param,ev);
        exit (127);
    }
    //wait(&status);
    
    close(fd[1]);
    out_fd = open(av[4],O_WRONLY | O_TRUNC | O_CREAT , 0644);
    if(out_fd == -1)
        return (3);
    dup2(fd[0], 0);
    dup2(out_fd, 1);
    close(out_fd);
    execve(out_command_path,out_exec_param,ev);
    return 127;
}