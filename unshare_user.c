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
#include <sys/types.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
	// Collect initial information about the process
	pid_t pid;
	uid_t uid;
	gid_t gid;
	pid=getpid();
	uid=getuid();
	gid=getgid();
	// Call unshare for new user namespace
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
	// While we are noname we have full privilage before an exec call
	// We should do the user and group mapping now.
	FILE * f;
	char path[256]; 
	sprintf(path,"/proc/%d/uid_map",pid);
	f=fopen(path,"w");
	if (f==NULL) printf("Error opening uid_map");
	ret=fprintf(f,"0 %d 1",uid);
	//TODO ret not handled
	fclose(f);
	sprintf(path,"/proc/%d/gid_map",pid);
	f=fopen(path,"w");
	if (f==NULL) printf("Error opening gid_map");
	ret=fprintf(f,"0 %d 1",gid);
	//TODO
	//execute our executable
	ret=execl("/bin/bash","bash",0,0);
	if (ret==-1)
	{
		printf("exec: %d\n",errno);
	}



	exit(EXIT_SUCCESS);
}
