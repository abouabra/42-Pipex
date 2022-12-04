#include "libft/ft_dprintf.h"
#include "libft/get_next_line.h"
#include "libft/libft.h"
#include "pipex.h"
#include <stdio.h>
#include <sys/fcntl.h>
#include <unistd.h>



int main(int ac,char **av, char **ev)
{
    char *total;
    char *tmp;

    total = "";
    if(ac != 3)
        return 0;
    
    while(1)
    {
        tmp = get_next_line(0);
        total = ft_strjoin(total, tmp);
        if(ft_strnstr(total, av[1], -1))
            break;
    }
    int fd = open(av[2],O_WRONLY | O_APPEND | O_CREAT, 0644);  
    if(fd == -1)
        return 0;
    ft_dprintf(fd, "%s", total);
    return 0;
}