#include <iostream>
#include <unistd.h>
#include <sys/wait.h> // Pour la fonction wait()

int main() {
    const int N = 3;
    std::cout << "main pid=" << getpid() << std::endl;
    
    int childCount = 0; // Compteur du nombre de fils créés
    
    for (int i = 1, j = N; i <= N && j == N && fork() == 0; i++) {
        if (fork() != 0){
            childCount++; //fils du pere
            break;
        } else {
            childCount = 0; //processus fils a 0 fils à la creation
        }
        std::cout << " i:j " << i << ":" << j << std::endl;
        
        for (int k = 1; k <= i && j == N; k++) {
            if (fork() == 0) {
                j = 0;
                std::cout << " k:j " << k << ":" << j << std::endl;
            }
            else {
                childCount++;
            }
        }
    }
    
    // Le processus père attend la fin de tous les processus fils
    while (childCount > 0) {
        int status;
        pid_t child_pid = wait(&status);
        if (child_pid > 0) {
            childCount--;
            std::cout << "Processus fils " << child_pid << " terminé" << std::endl;
        }
    }
    
    return 0;
}

