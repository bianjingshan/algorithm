#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <execinfo.h>
#include <pthread.h>
#include <execinfo.h>

#define BACKTRACE_SIZE 128

extern int add(int num);
extern void CreateTestThread();
void KillTestThread();

void dump(void)
{
        int i = 0, nptrs = 0;
        void *buf[BACKTRACE_SIZE];
        char **strings;
        nptrs = backtrace(buf, BACKTRACE_SIZE);

        printf("backtrace() returned %d addresses\n", nptrs);
        strings = backtrace_symbols(buf, nptrs);
        if (strings == NULL)
        {
                perror("backtrace_symbols");
                exit(EXIT_FAILURE);
        }
        for (i = 0; i < nptrs; i++)
        {
                printf(" [%02d] %s\n", i, strings[i]);
        }
        free(strings);
}

void signal_handler(int signo)
{
#if 0
        char buf[64] = {0};
        sprintf(buf, "cat /proc/%d/maps", getpid());
        system((const char*)buf);
#endif
        printf("\n=================>>>catch signal %d<<<=====================\n", signo);
        printf("Dump stack start...\n");
        dump();
        printf("Dump stack end...\n");
        signal(signo, SIG_DFL);
        raise(signo);
}

int main(int argc, char **argv)
{
        int sum = 0;
        signal(SIGSEGV, signal_handler);
        struct sigaction sa_usr;
        sa_usr.sa_flags = 0;
        sa_usr.sa_handler = signal_handler;   //信号处理函数
        sigaction(SIGUSR1, &sa_usr, NULL);

        CreateTestThread();
        sleep(1);
        KillTestThread();
        sleep(1);
        sum = add(3);
        printf("sum = %d\n", sum);
        return 0;
}

