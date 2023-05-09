#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

int	main(int argc, char **argv, char **env)
{
	char	*args[] = {"env", NULL};

	if (execve("/usr/bin/env", args, env) == -1)
		perror("error:");

	printf("an unexpected error occured");
}
