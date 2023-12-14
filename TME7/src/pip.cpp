#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

void myexec(int argc, char** argv){
	for (int i = 0; i < argc; i++){
		if (argv[i] != nullptr)
			std::cerr << argv[i];
		else break;
	}
	std::cerr << std::endl;
	if (execv(argv[0], argv) == -1){
		perror("exec");
		exit(3);
	}
}

int main(int argc, char** argv){
	int pfd[2];
	char* arg1[argc];
	char* arg2[argc];
	int s = 0;
	memset(arg1, 0,argc*sizeof(char*));
	memset(arg2,0,argc*sizeof(char*));
	for (int i = 1; i < argc; i++){ //arg1 = tous les arguments à gauche du |
		if (strcmp(argv[i], "|")==0){
			s = i+1;
			break;
		}
		else{

			arg1[i-1] = argv[i];
			//std::cerr << argv[i] << std::endl;
		}
	}

	for ( int i = 0; s < argc; s++, i++){ //arg2 = tous ceux à droite
		arg2[i] = argv[s];
		//std::cerr << argv[s] << std::endl;
	}

	pid_t pid;
	if (pipe(pfd) == -1){
		perror("pipe");
		exit(1);
	}
	//fils1
	if ((pid = fork()) == -1){
		perror("fork");
		exit(2);
	}

	if (pid == 0){
		dup2(pfd[1], STDOUT_FILENO);
		close(pfd[1]);
		close(pfd[0]);
		myexec(argc, arg1);
	}

	//fils2
	if ((pid = fork()) == -1){
		perror("fork");
		exit(2);
	}

	if (pid == 0){
		dup2(pfd[0], STDIN_FILENO);
		close(pfd[0]);
		close(pfd[1]);
		myexec(argc, arg2);
	}


	close(pfd[0]);
	close(pfd[1]);

	wait(0);
	wait(0);

	return 0;
}