#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
using namespace pr;

#define N 3
#define M 5

int stop = 1; //Flag CTRL C

void producteur (Stack<char> * stack) {
	char c ;
	while (cin.get(c) && stop) {
		stack->push(c);
	}
}

void consomateur (Stack<char> * stack) {
	while (stop) {
		char c = stack->pop();
		cout << c << flush ;
	}
}

int main () {
	//CTRL + C
	signal(SIGINT, [](int signal){
		stop = 0; //réception du ctrl+c -> met la variable stop à 0 pour arreter les productions/conso
	});

	//Mémoire partagée
	size_t len = sizeof(Stack<char>);
	int fd = shm_open("/myshm", O_CREAT|O_EXCL|O_RDWR, 0666);
	ftruncate(fd, len);
	void* addr = mmap(nullptr, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	Stack<char> * s = new (addr) Stack<char>();
	
	//Sémaphore
	vector<sem_t*> prod, cons;
	for (int i = 0; i < N; i ++){
		sem_t* s = sem_open(("/monprod" + to_string(i)).c_str(), O_CREAT|O_EXCL|O_RDWR, 0600, 1);
		prod.push_back(s);
	}
	for (int i = 0; i < M; i ++){
		sem_t* s = sem_open(("/moncons" + to_string(i)).c_str(), O_CREAT|O_EXCL|O_RDWR, 0600, 1);
		cons.push_back(s);
	}
	//Production
	for (int i = 0; i < N; i++){
		pid_t p = fork();
		if (p == 0){
			sem_wait(prod[i]);
			cout << "production " << i << endl;
			sem_post(cons[i % M]);
			exit(0);
		}
	}

	//Conso
	for (int i = 0; i < M; i++){
		pid_t q = fork();
		if ( q == 0){
			sem_wait(cons[i]);
			cout << "consommation " << i << endl;
			sem_post(prod[i % N]);
			exit(0);
		}
	}
	/*if (pp==0) {
		producteur(s);
		return 0;
	}

	pid_t pc = fork();
	if (pc==0) {
		consomateur(s);
		return 0;
	}*/

	for (int i = 0; i < N + M; i++){
		wait(nullptr);
	}

	/*wait(0);
	wait(0);
	*/

	// Nettoyage
	for (int i = 0; i < N; i++){
		sem_unlink(("/monprod" + to_string(i)).c_str());
	}

	for (int i = 0; i < M; i++){
		sem_unlink(("/moncons" + to_string(i)).c_str());
	}

    
    sem_unlink("/monsem0");
    sem_unlink("/monsem1");

	munmap(addr, len);
	shm_unlink("/myshm");
	return 0;
}

