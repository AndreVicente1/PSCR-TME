#include <iostream>
#include <csignal>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>



int hp = 3; // Compteur de vies

// Fonction de l'attaquant
void attaque(pid_t adversaire) {

    signal(SIGUSR1, [](int signal) {
        hp--;
        std::cout << getpid() << " est attaqué, points de vie restants : " << hp << std::endl;
        if (hp <= 0) {
            std::cout << getpid() << " est mort, le processus se termine." << std::endl;
            exit(1);
        }
    });

    
    if (kill(adversaire, SIGUSR1) != 0) {
        std::cout << "L'adversaire " << adversaire << " a déjà perdu. Le processus se termine." << std::endl;
        exit(0);
    }

    // sleep entre 0.3 et 1 seconde
    usleep((rand() % 700 + 300) * 1000);
}

// Fonction de défense
void defense() {
    signal(SIGUSR1, SIG_IGN); //ignoré le signal lors de la reception quand defense
    std::cout << getpid() << " defend " << std::endl;
    usleep((rand() % 700 + 300) * 1000);
}




// Fonction de combat entre Vador et Luke
void combat(pid_t adversaire) {
    while (true) {
        defense();
        attaque(adversaire);
        
    }
}

int main() {
    srand(time(NULL));

    pid_t luke_pid;
    pid_t vador_pid = getpid();
    std::cout << "Luke vs Vador" << std::endl;

    // Création du processus Luke
    if ((luke_pid = fork()) == 0) {
        combat(vador_pid); // Luke combat Vador
    } else {
        combat(luke_pid); // Vador combat Luke
    }
    wait(NULL);
    return 0;
}
