#include "Banque.h"
#include "iostream"
using namespace std;
using namespace pr;

const size_t N = 10; // Nombre de threads
void transactionThread(Banque& banque) {
    srand(time(NULL));

    for (int i = 0; i < 1000; ++i) {
        int idDebit = rand() % banque.size(); // indice compte
        int idCredit;
        do {
            idCredit = rand() % banque.size(); // indice compte different du premier
        } while (idDebit == idCredit);

        unsigned int val = (rand() % 100) + 1; // montant

        banque.transfert(idDebit, idCredit, val);
        this_thread::sleep_for(chrono::milliseconds(rand()%21));
    }
}

int main() {
    const size_t ncomptes = 100;
    const size_t soldeInitial = 1000;

    Banque banque(ncomptes, soldeInitial);
    vector<thread> threads;

    for (size_t i = 0; i < N; ++i) {
        threads.emplace_back(transactionThread, ref(banque));
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}