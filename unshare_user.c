/* demo_userns.c

   Copyright 2013, Michael Kerrisk
   Licensed under GNU General Public License v2 or later

   Demonstrate the use of the clone() CLONE_NEWUSER flag.

   Link with "-lcap" and make sure that the "libcap-devel" (or
   similar) package is installed on the system.
*/
#define _GNU_SOURCE
#include <sys/capability.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
	int ret;
	ret=unshare(//flags
			CLONE_NEWUSER
		   );
	if (ret==-1)
	{
		printf("unshare: ");
		switch (errno)
		{
			case EINVAL:
				printf("invalid bit\n");
				break;
			case ENOMEM:
				printf("memory error\n");
				break;
			case EPERM:
				printf("permission denied\n");
		}
	}
	ret=execl("/bin/bash","bash",0,0);
	if (ret==-1)
	{
		printf("exec: %d\n",errno);
	}
	exit(EXIT_SUCCESS);
}
