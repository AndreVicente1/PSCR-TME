#include "Banque.h"
#include "iostream"
using namespace std;
using namespace pr;

const int NB_THREAD = 10;
int main () {
	vector<thread> threads;
	int nb_comptes = 100;
	int JP = 1000;
	threads.reserve(NB_THREAD);
	Banque b(nb_comptes, JP);
	
	std::cout << "Creation  des threads" << std::endl;
	for(int i = 0; i < NB_THREAD; i++) {
		threads.push_back(thread([i, &b, nb_comptes](){
			for(int j = 0; j < 1000000; j++) {
				// transfert d'un montant m aleatoire entre 1 et 100 entre deux comptes aleatoire
				&Banque::transfert,&b,rand() % nb_comptes, rand() % nb_comptes, rand() % 100 + 1;
			}
			//dormir pendant une durée aléatoire entre 0 et 20ms
			this_thread::sleep_for(chrono::milliseconds(rand() % 20));	
		}));
	}
	std::cout << "Join  des threads" << std::endl;
	for (auto & t : threads) {
		t.join();
	}
	std::cout << "Fin des threads" << std::endl;


	// TODO : tester solde = NB_THREAD * JP
	std::cout << "Test du solde" << std::endl;
	
	if (b.comptabiliser( nb_comptes*JP)){
		std::cout << "Pas d'erreur de comptabilité" << std::endl;
	}

	return 0;
}
