#include <iostream>
#include <csignal>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h> // Pour la fonction wait()



int hp = 3; // Compteur de vies

// Fonction de l'attaquant
void attaque(pid_t adversaire) {

    signal(SIGUSR1, [](int signal) {
        if (signal == SIGUSR1){
            hp--;
            std::cout << getpid() << " attaque, points de vie restants : " << remainingLives << std::endl;
            if (hp <= 0) {
                std::cout << getpid() << " est mort, le processus se termine." << std::endl;
                exit(1);
            }
        }
    });

    
    if (kill(adversaire, SIGUSR1) != 0) {
        std::cout << "L'adversaire a déjà perdu. Le processus se termine." << std::endl;
        exit(0);
    }

    // sleep entre 0.3 et 1 seconde
    usleep((rand() % 700 + 300) * 1000);
}

// Fonction de défense
void defense() {
    signal(SIGUSR1, SIG_IGN);

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
    // Initialisation de la séquence aléatoire
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

    return 0;
}
