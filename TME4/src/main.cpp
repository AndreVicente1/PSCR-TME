#include "Banque.h"
#include <cstdlib>
#include <iostream>
using namespace std;
using namespace pr;

const int NB_THREAD = 10;
const int K = 100; // nb de comptes
const int SOLDEINITIAL = 70; // montant du compte à la creation

void transfertWork(int index, Banque b) {
	int i,j,m;
	for (int t=0 ; t<1000 ; ++t){
		cout << "thread " << index << endl;
		i = rand()%K;
		j = rand()%K;
		m = rand()%100+1;
		b.transfert(i, j, m);
		auto r = rand()%20;
		this_thread::sleep_for (chrono::milliseconds(r));
	}
}

void comptabiliserWork(Banque b){
	for (int i=0 ; i< 1000 ; i++) {
		b.comptabiliser(K*SOLDEINITIAL);
		auto r = rand()%20;
		this_thread::sleep_for (chrono::milliseconds(r));
	}
}

int main () {
	vector<thread> threads;
	threads.reserve(NB_THREAD + 1);

	Banque b(K,SOLDEINITIAL);
	::srand(::time(nullptr));

	for( int i=0 ; i<NB_THREAD ; i++){
		threads.emplace_back(transfertWork,i,  ref(b));
	}
	threads.emplace_back(comptabiliserWork, ref(b));

	for (auto & t : threads) {
		t.join();
	}

	return 0;
}