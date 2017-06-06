#define _GNU_SOURCE
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/socket.h>

#include <linux/filter.h>
#include <linux/seccomp.h>
#include <linux/audit.h>

#include "hashd.h"

#define ALLOW_SYSCALL(syscall)  BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, __NR_##syscall, 0, 1),  BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_ALLOW)

void sandbox_init(void);


void
sandbox_init(void)
{

    struct sock_filter filter[] = {
        BPF_STMT(BPF_LD+BPF_W+BPF_ABS,
                 offsetof(struct seccomp_data, arch)),
        BPF_JUMP(BPF_JMP+BPF_JEQ+BPF_K, AUDIT_ARCH_X86_64, 1, 0),
        BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_KILL),
        BPF_STMT(BPF_LD+BPF_W+BPF_ABS, offsetof(struct seccomp_data, nr)),
        /*allowed syscalls*/
        ALLOW_SYSCALL(exit_group),
        ALLOW_SYSCALL(brk),
        ALLOW_SYSCALL(mmap),
        ALLOW_SYSCALL(munmap),
        ALLOW_SYSCALL(write),
        ALLOW_SYSCALL(open),
        ALLOW_SYSCALL(lseek),
        ALLOW_SYSCALL(read),
        ALLOW_SYSCALL(close),
        ALLOW_SYSCALL(socket),
        ALLOW_SYSCALL(connect),
        ALLOW_SYSCALL(getuid),
        ALLOW_SYSCALL(geteuid),
        ALLOW_SYSCALL(getgid),
        ALLOW_SYSCALL(getegid),
        ALLOW_SYSCALL(fcntl),
        ALLOW_SYSCALL(eventfd2),
        ALLOW_SYSCALL(rt_sigaction),
        ALLOW_SYSCALL(bind),
        ALLOW_SYSCALL(getsockname),
        ALLOW_SYSCALL(sendto),
        ALLOW_SYSCALL(recvmsg),
        ALLOW_SYSCALL(setsockopt),
        ALLOW_SYSCALL(listen),
        ALLOW_SYSCALL(accept4),
        ALLOW_SYSCALL(recvfrom),
        ALLOW_SYSCALL(epoll_create1),
        ALLOW_SYSCALL(epoll_ctl),
        ALLOW_SYSCALL(epoll_wait),
        /*die if process tries to execute any other syscall*/
        BPF_STMT(BPF_RET+BPF_K, SECCOMP_RET_KILL),
    };

    struct sock_fprog filterprog = {
        .len = sizeof(filter)/sizeof(filter[0]),
        .filter = filter
    };

    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
        perror("Could not start seccomp:");
        exit(1);
    }
    if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &filterprog) == -1) {
        perror("Could not start seccomp:");
        exit(1);
    }
}
