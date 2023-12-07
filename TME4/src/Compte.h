#pragma once
#include <thread>
#include <mutex>

namespace pr {

class Compte {
	mutable std::mutex m;
	int solde;

	//Question 10
	bool traitement = false;
public :
	Compte(int solde=0):solde(solde) {}
	Compte(const Compte & other);
	void crediter (unsigned int val) ;
	bool debiter (unsigned int val) ;
	int getSolde() const  ;
	std::mutex & getMutex();

	//Question 10
	void debut_traitement();
	void fin_traitement();
	bool est_traitement() const;
};

}
