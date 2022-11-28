#include "libft/libft.h"
#include "pipex.h"
#include <stdio.h>
#include <sys/unistd.h>
#include <unistd.h>


int main(int ac,char **av, char **ev)
{
    char **comamnds = ft_split(av[1], ' ');
    char *path = "./script\"quote.sh";
    int r = access(path, F_OK);
    printf("RETURN: %d\n",r);
    execve(path, comamnds, ev);
    printf("HAHA");
    return 0;
}