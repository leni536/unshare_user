all: unshare_user

unshare_user: unshare_user.c
	gcc -o unshare_user -I /usr/include/x86-64-linux-gnu -lcap unshare_user.c
