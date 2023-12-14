#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

pid_t wait_till_pid(pid_t pid) {
    int status;
    while (true){
        pid_t pidt = wait(&status);
        if (pidt == pid) return pidt;
    }
}

pid_t wait_till_pid(pid_t pid, int sec){
    signal(SIGCHLD, [](int signal){

    });
    signal(SIGALRM, [](int signal){

    });
    
}