#include "Banque.h"

#include <iostream>
#include <algorithm>

using namespace std;

namespace pr {

/*void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
	Compte & debiteur = comptes[deb];
	Compte & crediteur = comptes[cred];
	debiteur.getMutex().lock();
	crediteur.getMutex().lock();
	if (debiteur.debiter(val)) {
		crediteur.crediter(val);
	}
	crediteur.getMutex().unlock();
	debiteur.getMutex().unlock();
}*/

void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
	//Question 9: rajouter un mutex
	//std::unique_lock<std::mutex> lock(mutex);

    size_t first = std::min(deb, cred);
    size_t second = std::max(deb, cred);

    Compte &compteA = comptes[first];
    Compte &compteB = comptes[second];

	//Question 10
	if (compteA.est_traitement() || compteB.est_traitement()) { //En cours de traitement, donc pas d'opération
		return;
	}

    std::lock(compteA.getMutex(), compteB.getMutex());
    std::lock_guard<std::recursive_mutex> lockA(compteA.getMutex(), std::adopt_lock);
    std::lock_guard<std::recursive_mutex> lockB(compteB.getMutex(), std::adopt_lock);

    if (deb == first) {
        if (compteA.debiter(val)) {
            compteB.crediter(val);
        }
    } else {
        if (compteB.debiter(val)) {
            compteA.crediter(val);
        }
    }
}

size_t Banque::size() {
	return comptes.size();
}
bool Banque::comptabiliser (int attendu) const {
	//Question 9 : rajouter un mutex
	//std::unique_lock<std::mutex> lock(mutex);

	int bilan = 0;
	int id = 0;
	for (const auto & compte : comptes) {
		compte.debut_traitement() //Question 10
		unique_lock<mutex> lock(compte.getMutex());
		if (compte.getSolde() < 0) {
			cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
		}
		bilan += compte.getSolde();
		id++;
		compte.fin_traitement() //Question 10
	}
	if (bilan != attendu) {
		cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
	}
	return bilan == attendu;
}
}
