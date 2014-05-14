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

static int
childFunc(void * arg_uid, void * arg_gid)
{
	int ret;
	uid_t * uid=arg_uid;
	gid_t * gid=arg_gid;
	FILE  * f;
	//mount proc to standard location
	ret=mount("proc","/proc","proc",0,0);
	if (ret==-1) printf("mount: %d",errno);


	f=fopen("/proc/1/uid_map","w");
	if (f==NULL) printf("Error opening uid_map");
	ret=fprintf(f,"%d %d 1",*uid);
	//TODO ret not handled
	fclose(f);
	f=fopen("/proc/1/gid_map","w");
	if (f==NULL) printf("Error opening gid_map");
	ret=fprintf(f,"%d %d 1",*gid);
	//TODO
	fclose(f);


	//execute our executable
	ret=execlp("bash","bash",0,0);
	if (ret==-1)
	{
		printf("exec: %d\n",errno);
	}
}

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];    /* Space for child's stack */

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
			CLONE_NEWUSER	|
			CLONE_NEWNS
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
	fclose(f);

	//clone for new pid namespace
	pid_t chd_pid;
	chd_pid=clone(childFunc,child_stack+STACK_SIZE,
			CLONE_NEWPID	|
			CLONE_NEWUSER	|
			SIGCHLD
			,&uid,&gid,0,0);
	if (chd_pid==-1) printf("clone: %d", errno);

	if (waitpid(chd_pid, NULL, 0) == -1)      /* Wait for child */
	    printf("waitpid");
	
	exit(EXIT_SUCCESS);
}
