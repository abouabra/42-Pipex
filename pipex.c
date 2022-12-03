
#include "pipex.h"
#include <stdio.h>


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
    
    char **exec_param;
    char *command_path;
    int fd[2];
    int index;
    //printf("IN Command: %s\n",av[2]);
    //printf("OUT Command: %s\n",av[3]);


    if(pipe(fd) == -1)
        return 1;
    index =0;
    
    while (++index < ac - 2)
    {

        if(pipe(fd)== -1)
            return 1;
        exec_param = ft_split(av[index], ' ');
        command_path = check_command_path(*exec_param, ev);
        if(index > 1 && !command_path)
        {
                
            char *str;
            char *str2 = ft_strjoin(ft_strdup("pipex: "),*exec_param);
            str = ft_strjoin(str2,ft_strdup(": command not found\n"));
            ft_putstr_fd(str, 2);
        }
        
        if(index > 1 && command_path && access(command_path, X_OK) == -1)
        {
            char *str;
            str = ft_strjoin(ft_strdup("pipex: "),"permission denied: ");
            str = ft_strjoin(str,*exec_param);
            str = ft_strjoin(str,"\n");
            ft_putstr_fd(str, 2);
        }
        int id = fork();
        if(id == -1)
            return 1;
        if(id==0)
        {
            dup2(fd[1],1);
         
            close(fd[0]);
            close(fd[1]);

            execve(command_path, exec_param, ev);
            exit(127);
        }
        else
        {
            if(index == 1)
            {
                int infd = open(av[1], O_RDONLY | O_CREAT , 0644);
                // if(infd == -1)
                //     return (1);
                dup2(infd,0);
                close(infd);
            }
            else
                dup2(fd[0],0);

            close(fd[1]);
            close(fd[0]);
            //waitpid(id, NULL ,0);
        }
    }


    close(fd[1]);
    int out_fd = open(av[index+1], O_WRONLY | O_TRUNC | O_CREAT , 0644);
    if(out_fd == -1)
        return (1);
    dup2(out_fd,1);
    close(out_fd);
    
    exec_param = ft_split(av[index], ' ');
    command_path = check_command_path(*exec_param, ev);
    if(!command_path)
    {
            
        char *str;
        char *str2 = ft_strjoin(ft_strdup("pipex: "),*exec_param);
        str = ft_strjoin(str2,ft_strdup(": command not found\n"));
        ft_putstr_fd(str, 2);
    }
    
    if(command_path && access(command_path, X_OK) == -1)
    {
        char *str;
        str = ft_strjoin(ft_strdup("pipex: "),"permission denied: ");
        str = ft_strjoin(str,*exec_param);
        str = ft_strjoin(str,"\n");
        ft_putstr_fd(str, 2);
    }
    execve(command_path, exec_param, ev);
    if(command_path && access(command_path, X_OK) == -1)
        return 126;
    return 127;
}