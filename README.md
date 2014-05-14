unshare\_user
============

A small c software for creating a PID namespace with an unprivileged user.

usage
=====

Call unshare_user without arguments with a normal user. It will open a bash process in a separate linux process namespace. Its more like an example then a useful software, there are lots of error checking missing. The method could be useful for configuring ssh sessions so the logged in users can't see each others running processes.

notes
===

The metod relies upon that since linux 3.8 it's possible to call unshare and clone with the CLONE\_NEWUSER flag without the CAP\_SYS\_ADMIN capability. However this capability is disabled by default on Debian, but you can reenable it by calling "echo 1>/proc/sys/kernel/unprivileged\_userns\_clone".
